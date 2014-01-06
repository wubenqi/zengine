// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net2/module_net2_plugin.h"

#include "base/logging.h"
#include "base/stl_util.h"

//#include "message/m"
#if defined(OS_WIN)
#include "net/base/winsock_init.h"
#endif
#include "net/engine/reactor.h"
#include "net/engine/net_engine_manager.h"

#include "zengine/config_file.h"
#include "zengine/zengine_context.h"
#include "zengine/zengine_context_manager.h"
#include "zengine/script/script_manager.h"
#include "mod_net/znet_comm_handler.h"

#include "mod_net2/register_helper/net2_register_helper.h"

namespace zengine {

const std::string kNe2tModuleName("mod_net2");

const std::string& ModuleNet2Plugin::GetModuleNet2Name() {
  return kNe2tModuleName;
}

//////////////////////////////////////////////////////////////////////////
ModuleNet2Plugin::ModuleNet2Plugin()
  : net_engine_manager_(NULL) {

}

ModuleNet2Plugin::~ModuleNet2Plugin() {
  STLDeleteElements(&conns_);
}

const std::string& ModuleNet2Plugin::GetPluginName() const {
  return kNe2tModuleName;
}

void ModuleNet2Plugin::Install() {
  ConfigFile* config = ConfigFile::GetInstance();
  if (config->CheckByValue("Modules", "file_path", "mod_net")) {
    LOG(ERROR) << "mod_net2 not use mod_net";
    return;
  }
  if (!config->CheckByValue("Modules", "file_path", "mod_protobuf")) {
    LOG(ERROR) << "mod_net2 depend mod_protobuf module";
    return;
  }

#if defined(OS_WIN)
  net::EnsureWinsockInit();
#endif
  Net2_Register_Helper();
  net_engine_manager_ = net::NetEngineManager::GetInstance();
}

void ModuleNet2Plugin::Initialize() {
  // 初始化网络
  net_engine_manager_->Initialize(1, this);
  net_engine_manager_->Run();


  ConfigFile* config = ConfigFile::GetInstance();

  zengine::StringVector instances = config->GetStringList(kNe2tModuleName.c_str(), "instance_name");
  for (size_t i=0; i<instances.size(); ++i) {
    if (config->CheckBySection(instances.string_vector[i].c_str())) {
      CommNodeInfo* comm_node2 = new CommNodeInfo();
      CommNodeInfo& comm_node = *comm_node2;
      const char* val = NULL;
      comm_node.instance_name = instances.string_vector[i];
      val = config->GetString(instances.string_vector[i].c_str(), "net_type");
      if (!val) { LOG(ERROR) << "ModuleNet2Plugin::Initialize - CommNodeInfo:" << instances.string_vector[i] << ":host is empty."; break; }
      comm_node.net_type = val;
      if (!comm_node.CheckNetType())  { LOG(ERROR) << "ModuleNet2Plugin::Initialize - net_type:" << val << "is invalid, shuold is " << CommNodeInfo::kNetType_TCPAcceptor << " or " << CommNodeInfo::kNetType_TCPConnector; }

      val = config->GetString(instances.string_vector[i].c_str(), "addr_info");
      if (!val) { LOG(ERROR) << "ModuleNet2Plugin::Initialize - CommNodeInfo:" << instances.string_vector[i] << ":user is empty."; break; }
      comm_node.addr = val;
      size_t pos = comm_node.addr.find_first_of(':');
      if (pos==std::string::npos) {
        LOG(ERROR) << "ModuleNet2Plugin::Initialize - addr_info:" << val << "is invalid, not sep char ':'";
      }
      comm_node.port = comm_node.addr.substr(pos+1, comm_node.addr.length()-pos-1);
      comm_node.addr = comm_node.addr.substr(0, pos);

      val = config->GetString(instances.string_vector[i].c_str(), "remote_passwd");
      if (val) {
        comm_node.auth_password = val;
      }
      comm_node.reconnected_timeout = config->GetInt(instances.string_vector[i].c_str(), "reconnected_time", 10);

      // 直接连吧
      comm_nodes_.push_back(comm_node2);
      //CommNodeInfo* t = &(*comm_nodes_.rbegin());
      if (comm_node.NetTypeIsTCPConnector()) {
        net::TCPConnectorPtr conn = new net::TCPConnector(net_engine_manager_, ZNetCommHandlerFactory::GetInstance(), comm_node2);
        conn->Connect(comm_node.addr, comm_node.port, true, true, comm_node.reconnected_timeout);
        conns_.push_back(conn);
      } else {
        net::TCPAcceptorPtr acc = new net::TCPAcceptor(net_engine_manager_, ZNetCommHandlerFactory::GetInstance(), comm_node2);
        if(!acc->Create(comm_node.addr, comm_node.port, true)) {
          LOG(ERROR) << "ModuleNet2Plugin::Initialize - Create TCPAcceptor server " << comm_node.addr << ":" << comm_node.port << " error!";
        } else {
          accs_.push_back(acc);
        }
      }
    }
  }

}

void ModuleNet2Plugin::Shutdown() {
  if (net_engine_manager_) {
    net_engine_manager_->Shutdown();
    net_engine_manager_ = NULL;
  }
  conns_.clear();
  accs_.clear();
}

void ModuleNet2Plugin::Uninstall() {
}

bool ModuleNet2Plugin::CreateTCPConnector(const std::string& addr, const std::string& port, int reconnector_timeout, const std::string& instance_name) {
  if (net_engine_manager_==NULL) {
    LOG(ERROR) << "NetEngine not initialize, must execute zengine:Initialize_NetEngineManager()";
    return false;
  }

  CommNodeInfo* comm_node2 = new CommNodeInfo();
  CommNodeInfo& comm_node = *comm_node2;
  comm_node.instance_name = instance_name;
  comm_node.addr = addr;
  comm_node.port = port;
  comm_node.reconnected_timeout = reconnector_timeout;
  comm_node.net_type = CommNodeInfo::kNetType_TCPConnector;
  comm_nodes_.push_back(comm_node2);

  net::TCPConnectorPtr conn = new net::TCPConnector(net_engine_manager_, ZNetCommHandlerFactory::GetInstance(), comm_node2);
  conn->Connect(addr, port, true, true, reconnector_timeout);
  conns_.push_back(conn);
  return true;
}

bool ModuleNet2Plugin::CreateTCPAcceptor(const std::string& addr, const std::string& port, const std::string& instance_name) {
  if (net_engine_manager_==NULL) {
    LOG(ERROR) << "NetEngine not initialize, must execute zengine:Initialize_NetEngineManager()";
    return false;
  }

  CommNodeInfo* comm_node2 = new CommNodeInfo();
  CommNodeInfo& comm_node = *comm_node2;
  comm_node.instance_name = instance_name;
  comm_node.addr = addr;
  comm_node.port = port;
  //comm_node.reconnected_timeout = 10;
  comm_node.net_type = CommNodeInfo::kNetType_TCPAcceptor;
  comm_nodes_.push_back(comm_node2);

  net::TCPAcceptorPtr acc = new net::TCPAcceptor(net_engine_manager_, ZNetCommHandlerFactory::GetInstance(), comm_node2);
  if(!acc->Create(addr, port, true)) {
    LOG(ERROR) << "ModuleNet2Plugin::CreateTCPAcceptor - Create TCPAcceptor server " << comm_node.addr << ":" << comm_node.port << " error!";
    return false;
  }
  accs_.push_back(acc);
  return true;
}

void ModuleNet2Plugin::OnNetEngineCreate(net::Reactor* reactor) {
  LOG(INFO) << "ModuleNet2Plugin::OnNetEngineCreate(" << reactor->thread_name() << ")";
  ZEngineContext* context = ZEngineContextManager::GetInstance()->CreateContext(ZEngineContext::kZEngineContextType_IO, base::PlatformThread::CurrentId(), ZEngineContext::kIOInstanceName, reactor->message_loop());
  reactor->AttachUserData(context);
  context->script_manager()->ExeScript_OnThreadInitialize(context);
}

void ModuleNet2Plugin::OnNetEngineDestroy(net::Reactor* reactor) {
  LOG(INFO) << "ModuleNet2Plugin::OnNetEngineDestroy(" << reactor->thread_name() << ")";
  ZEngineContext* context = reinterpret_cast<ZEngineContext*>(reactor->DetachUserData());
  // 这里可能有问题
  context->script_manager()->ExeScript_OnThreadDestroy(context);
  context->Destroy();
}

//////////////////////////////////////////////////////////////////////////
const std::string& ModuleNet2PluginFactory::GetPluginName() const {
  return kNe2tModuleName;
}

Plugin* ModuleNet2PluginFactory::CreateInstance() {
  return new ModuleNet2Plugin();
}

void ModuleNet2PluginFactory::DestroyInstance(Plugin* module) {
  ModuleNet2Plugin* net_db = reinterpret_cast<ModuleNet2Plugin*>(module);
  if (net_db) {
    delete net_db;
    net_db = NULL;
  }
}

}
