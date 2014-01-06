// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_daemon.h"

#include "base/at_exit.h"
#include "base/sys_info.h"
#include "base/bind.h"
#include "base/stl_util.h"
#include "base/file_util.h"
#include "base2/arg.h"

//#include "message/m"
#if defined(OS_WIN)
#include "net/base/winsock_init.h"
#endif
#include "net/engine/reactor.h"
#include "net/engine/net_engine_manager.h"

//#include "zengine/client_comm_handler.h"
#include "zengine/config_file.h"


#include "zengine/zengine_root.h"
#include "zengine/script/script_file_manager.h"
#include "zengine/script/register_helper/base_register_helper.h"
#include "zengine/script/register_helper/zengine_register_helper.h"

//static std::string kDefaultAppName("test");
//
//void Help() {
//  std::cout<<"zengine.\nCopyright (C) 2012 www.lelewan.com\n\n"
//    "Usage: zengine --app=app_name [--daemon] [-h|--help] \n\n"
//    "\t--app=app_name: zengine app name\n"
//    "\t[--daemon]: Run as daemon(in linux platform)\n"
//    "\t[-h|--help]: print me.\n"
//    <<std::endl;
//}

ZEngineDaemon::ZEngineDaemon() 
  : BaseDaemon() {

  //net_engine_manager_ = NULL;
  //script_manager_ = NULL;

  zengine::Root::GetInstance();
}

ZEngineDaemon::~ZEngineDaemon() {
}

int ZEngineDaemon::LoadConfig( const FilePath& xml_ini_file ) {
  ConfigFile* config_file = ConfigFile::GetInstance();
  return config_file->Initialize(xml_ini_file);
}

int	ZEngineDaemon::Initialize( int argc, char** argv ) {  
  // 脚本初始化
  ConfigFile* config_file = ConfigFile::GetInstance();
  zengine::StringVector values = config_file->GetStringList("Scripts", "file_path");

  std::vector<FilePath> script_file_paths;
  for (size_t i=0; i<values.size(); ++i) {
    script_file_paths.push_back(FilePath::FromUTF8Unsafe(values.string_vector[i]));
  }
  ScriptFileManager::GetInstance()->Initialize(script_file_paths);
  
  zengine::Base_Register_Helper();
  zengine::ZEngine_Register_Helper();

  zengine::Root::GetInstance()->Initialize(config_file);

  //context_manager_ = ZEngineContextManager::GetInstance();
  //// DCHECK(context_manager_);
  //
  //context_ = context_manager_->CreateContext(ZEngineContext::kZEngineContextType_Main, base::PlatformThread::CurrentId(), ZEngineContext::kMainInstanceName, message_loop());
  ////context_manager_->PushBack(context_);
  ////context_->Initialize(app_name_);

  //context_->script_manager()->ExeScript_MainInitialize(this);
  //context_->script_manager()->ExeScript_OnThreadInitialize(context_);

  return 0;

#if 0
  //uint64 t = MAKEINT64(1,1);
  //LOG(INFO) << t;

  // 客户端连接处理
  if(!ClientCommManager::GetInstance()->Initialize( net_engine_manager_, ConfigInfo::GetInstance()->client_node().addr.addr, ConfigInfo::GetInstance()->client_node().addr.port)) {

    LOG(ERROR) << "Start server in port[" << ConfigInfo::GetInstance()->client_node().addr.port << "] error.";
    return -1;
  }
  LOG(INFO) << "Client server in port["<< ConfigInfo::GetInstance()->client_node().addr.port << "]";

  // 客户端连接处理
  if(!ServerCommManager::GetInstance()->Initialize(net_engine_manager_, ConfigInfo::GetInstance()->server_node().addr.addr, ConfigInfo::GetInstance()->server_node().addr.port)) {
    LOG(ERROR) << "Start server in port[" << ConfigInfo::GetInstance()->server_node().addr.port << "] error.";
    return -1;
  }
  LOG(INFO) << "Server server in port["<< ConfigInfo::GetInstance()->server_node().addr.port << "]";
#endif

  // return Initialize_NetEngineManager();
}

int ZEngineDaemon::Run() {
  zengine::Root::GetInstance()->Run();
  return 0;
}

#if 0
void ZEngineDaemon::Initialize_NetEngineManager() {
#if defined(OS_WIN)
  net::EnsureWinsockInit();
#endif

  if (!net_engine_manager_) {
    // 初始化网络
    net_engine_manager_ = net::NetEngineManager::GetInstance();
    net_engine_manager_->Initialize(1, this);
    net_engine_manager_->Run();
  }
  
}

bool ZEngineDaemon::CreateTCPConnector(const std::string& addr, const std::string& port, int reconnector_timeout) {
  if (net_engine_manager_==NULL) {
    LOG(ERROR) << "NetEngine not initialize, must execute zengine:Initialize_NetEngineManager()";
    return false;
  }
  
  net::TCPConnectorPtr conn = new net::TCPConnector(net_engine_manager_, ClientCommHandlerFactory::GetInstance(), this);
  if(!conn->Connect(addr, port, true, true, reconnector_timeout)) {
    // return -1;
  }
  conns_.push_back(conn);
  return true;
}

bool ZEngineDaemon::CreateTCPAcceptor(const std::string& addr, const std::string& port) {
  if (net_engine_manager_==NULL) {
    LOG(ERROR) << "NetEngine not initialize, must execute zengine:Initialize_NetEngineManager()";
    return false;
  }

  net::TCPAcceptorPtr acc = new net::TCPAcceptor(net_engine_manager_, ClientCommHandlerFactory::GetInstance(), this);
  if(!acc->Create(addr, port, true)) {
    return false;
  }
  accs_.push_back(acc);
  return true;
}
#endif

#if 0
int ZEngineDaemon::Initialize_NetEngineManager() {
  //// 初始化网络
  //net_engine_manager_ = net::NetEngineManager::GetInstance();
  //net_engine_manager_->Initialize(1, this);
  //net_engine_manager_->Run();

  ConfigInfo* config_info = ConfigInfo::GetInstance();
  std::vector<CommNodeInfo>& comm_nodes = config_info->GetCommNodeInfos();

  for (size_t i=0; i<comm_nodes.size(); ++i) {
    CommNodeInfo& comm_node = comm_nodes[i];
    if (comm_node.IsConnectServiceType()) {
      net::TCPConnectorPtr conn = new net::TCPConnector(net_engine_manager_, ClientCommHandlerFactory::GetInstance(), this);
      if(!conn->Connect(comm_node.addr, comm_node.port, true, true, comm_node.reconnected_timeout)) {
        // return -1;
      }
      conns_.push_back(conn);
    } else if (comm_node.IsBindServiceType()) {
      net::TCPAcceptorPtr acc = new net::TCPAcceptor(net_engine_manager_, ClientCommHandlerFactory::GetInstance(), this);
      if(!acc->Create(comm_node.addr, comm_node.port, true)) {
        //return -1;
      }
      accs_.push_back(acc);
    }
  }

  return 0;
}
#endif

//bool ZEngineDaemon::CreateZEngineThread(const std::string& instance_name) {
//  if (instance_name==ZEngineContext::kIOInstanceName || instance_name==ZEngineContext::kMainInstanceName) {
//    LOG(ERROR) << "CreateZEngineThread error, instance_name is " << instance_name << ", not equsal main or io.";
//    return false;
//  }
//
//  if (zengine_threads_.find(instance_name) != zengine_threads_.end()) {
//    LOG(ERROR) << "CreateZEngineThread error, instance_name " << instance_name << " existed.";
//    return false;
//  }
//  
//  base::SharedPtr<ZEngineThread> zthread(new ZEngineThread(instance_name, context_manager_));
//  zthread->Start();
//  zengine_threads_.insert(std::make_pair(instance_name, zthread));
//
//  return true;
//}

//ZEngineThread* ZEngineDaemon::GetZEngineThread(const std::string& instance_name) {
//  ZEngineThread* zthread = NULL;
//  
//}

int ZEngineDaemon::Destroy() {

  // context_->script_manager()->ExeScript_MainInitialize(this);
  // context_->script_manager()->ExeScript_OnThreadInitialize(context_);

  zengine::Root::GetInstance()->Shutdown();

  //ZEngineThreadMap::iterator it;
  //for (it=zengine_threads_.begin(); it!=zengine_threads_.end(); ++it) {
  //  it->second->Stop();
  //}
  //zengine_threads_.clear();


  // main_script_manager_.reset();
  // base::hash_map<std::string, base::SharedPtr<ZEngineThread> >::iterator it
  //if (context_) {
  //  context_->script_manager()->ExeScript_OnThreadDestroy(context_);
  //  context_->script_manager()->ExeScript_MainDestroy(this);

  //  context_->Destroy();
  //  delete context_;
  //  context_ = NULL;
  //}
  //if(context_manager_) {
  //  context_manager_->Destroy();
  //  delete context_manager_;
  //  context_manager_ = NULL;
  //}

  return 0;
}

#if 0
void ZEngineDaemon::OnNetEngineCreate(net::Reactor* reactor) {
  LOG(INFO) << "ZEngineDaemon::OnNetEngineCreate(" << reactor->thread_name() << ")";
  ZEngineContext* context = context_manager_->CreateContext(base::PlatformThread::CurrentId(), ZEngineContext::kIOInstanceName, reactor);
  reactor->AttachUserData(context);
  context->script_manager()->ExeScript_OnThreadInitialize(context);
}

void ZEngineDaemon::OnNetEngineDestroy(net::Reactor* reactor) {
  LOG(INFO) << "ZEngineDaemon::OnNetEngineDestroy(" << reactor->thread_name() << ")";
  ZEngineContext* context = reinterpret_cast<ZEngineContext*>(reactor->DetachUserData());
  
  // 这里可能有问题
  context->script_manager()->ExeScript_OnThreadDestroy(context);
  context->Destroy();
}
#endif

int main(int argc, char* argv[]) {
  base::AtExitManager at_exit_manager;

  ZEngineDaemon daemon;
  return daemon.DoMain(argc, argv);
}

