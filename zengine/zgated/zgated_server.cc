// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zgated/zgated_server.h"

#include "base/at_exit.h"
#include "base/sys_info.h"

#if defined(OS_WIN)
#include "net/base/winsock_init.h"
#endif
#include "net/engine/net_engine_manager.h"

#include "zgated/config_info.h"

#include "zgated/client_comm_handler.h"
#include "zgated/client_comm_manager.h"
#include "zgated/server_comm_handler.h"
#include "zgated/server_comm_manager.h"


ZGatedServer::ZGatedServer() 
  : BaseDaemon() {
  net_engine_manager_ = NULL;
}

ZGatedServer::~ZGatedServer() {
}

int ZGatedServer::LoadConfig( const FilePath& xml_ini_file ) {
  return ConfigInfo::GetInstance()->Initialize(xml_ini_file);
}

int	ZGatedServer::Initialize( int argc, char** argv ) {
  net_engine_manager_ = net::NetEngineManager::GetInstance();
  net_engine_manager_->Initialize(base::SysInfo::NumberOfProcessors());
  net_engine_manager_->Run();

  //uint64 t = MAKEINT64(1,1);
  //LOG(INFO) << t;

  // 客户端连接处理
  if(!ClientCommManager::GetInstance()->Initialize( net_engine_manager_, ConfigInfo::GetInstance()->client_node().addr, ConfigInfo::GetInstance()->client_node().port)) {

    LOG(ERROR) << "Start server in port[" << ConfigInfo::GetInstance()->client_node().port << "] error.";
    return -1;
  }
  LOG(INFO) << "Client server in port["<< ConfigInfo::GetInstance()->client_node().port << "]";

  // 客户端连接处理
  if(!ServerCommManager::GetInstance()->Initialize(net_engine_manager_, ConfigInfo::GetInstance()->server_node().addr, ConfigInfo::GetInstance()->server_node().port)) {
    LOG(ERROR) << "Start server in port[" << ConfigInfo::GetInstance()->server_node().port << "] error.";
    return -1;
  }
  LOG(INFO) << "Server server in port["<< ConfigInfo::GetInstance()->server_node().port << "]";

  return 0;
}

int ZGatedServer::Destroy() {
  net_engine_manager_->Shutdown();
  return 0;
}

int main(int argc, char* argv[]) {
  base::AtExitManager at_exit_manager;

#if defined(OS_WIN)
  net::EnsureWinsockInit();
#endif

  ZGatedServer daemon;
  return daemon.DoMain(argc, argv);
}

