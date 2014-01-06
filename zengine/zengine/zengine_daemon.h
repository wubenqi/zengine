// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_DAEMON_H_
#define ZENGINE_ZENGINE_DAEMON_H_
#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/hash_tables.h"

#include "base2/daemon/base_daemon.h"

//#include "net/base/io_buffer.h"
//#include "net/engine/net_engine_manager.h"
//#include "net/engine/tcp_acceptor.h"
//#include "net/engine/tcp_connector.h"
//#include "zengine/zengine_context_manager.h"

//class ScriptEngine;
class ScriptManager;
class ZEngineThread;
class ZEngineContext;
class ZEngineContextManager;
class ZEngineDaemon :
  public base::BaseDaemon/*,
  public net::NetEngineManagerDelegate*/ {
public:
  ZEngineDaemon();
  virtual ~ZEngineDaemon();

  //ScriptManager* GetIOScriptManager(size_t reactor_id) {
  //  if (reactor_id<io_script_managers_.size()) {
  //    return io_script_managers_[reactor_id].get();
  //  }
  //  return NULL;
  //}

  //void Initialize_NetEngineManager();

  //bool CreateTCPConnector(const std::string& addr, const std::string& port, int reconnector_timeout);
  //bool CreateTCPAcceptor(const std::string& addr, const std::string& port);

  //bool CreateZEngineThread(const std::string& instance_name);

  //bool PostTaskToZEngineThread(const std::string& instance_name, const std::string& task_data);
  //bool PostTaskToIOThread(const std::string& task_data);
  //void PostTaskToMainThread(const std::string& task_data);

protected:
  // 来自 base::BaseDaemon
  virtual int LoadConfig( const FilePath& xml_ini_file );
  virtual int	Initialize( int argc, char** argv );
  virtual int Destroy();
  virtual int Run();

  // 来自 net::NetEngineManagerDelegate
  //virtual void OnNetEngineCreate(net::Reactor* reactor);
  //virtual void OnNetEngineDestroy(net::Reactor* reactor);

private:
  //typedef base::hash_map<std::string, base::SharedPtr<ZEngineThread> > ZEngineThreadMap;

  //net::NetEngineManager* net_engine_manager_;

  //std::vector<net::TCPAcceptorPtr> accs_;
  //std::vector<net::TCPConnectorPtr> conns_;

/*
  scoped_ptr<ScriptManager> main_script_manager_; // 主线程脚本管理器
  std::vector<base::SharedPtr<ScriptManager> > io_script_managers_; // 网络线程脚本管理器（reactor_id作为索引）
  */
  //ZEngineThreadMap zengine_threads_;
  //ZEngineContext* context_;
  //ZEngineContextManager* context_manager_;
};


#endif
