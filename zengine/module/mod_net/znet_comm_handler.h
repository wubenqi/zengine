// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_NET_ZNET_COMM_HANDLER_H_
#define MOD_NET_ZNET_COMM_HANDLER_H_
#pragma once

#include "base/memory/singleton.h"

#include "net/hammer/hammer_io_handler.h"
#include "net/engine/io_handler_factory.h"

//class ZEngineDaemon;
//class ScriptManager;

namespace net {
class Reactor;
}

class ZEngineContext;
struct CommNodeInfo;
class ZNetCommHandler :
  public net::HammerIOHandler {
public:
  ZNetCommHandler(SOCKET socket, net::Reactor* reactor);
  virtual ~ZNetCommHandler();
  void Destroy();

  //////////////////////////////////////////////////////////////////////////
  virtual int OnNewConnection();
  virtual int OnDataReceived(const PacketPtr& packet);
  virtual int OnConnectionClosed();

  ZEngineContext* GetContext() {
    return context_;
  }
  
  const std::string& GetInstanceName()  const;
  const std::string& GetNetType()  const;

  const std::string& GetReadableSessionID() const {
    return string_session_id_;
  }

  void set_conn_state(int conn_state) {
    conn_state_ = conn_state;
  }

  int conn_state() const {
    return conn_state_;
  }

  virtual void SetUserData(void* user_data);

private:
  int conn_state_;
  std::string log_prefix_;
  ZEngineContext* context_;
  std::string string_session_id_;
  CommNodeInfo* comm_node_info_;
  //ZEngineDaemon* zengine_daemon_;
  //ScriptManager* script_manager_;
};

class ZNetCommHandlerFactory :
  public net::IOHandlerFactory {
public:
  static ZNetCommHandlerFactory* GetInstance() {
    return Singleton<ZNetCommHandlerFactory>::get();
  }

  virtual net::IOHandler* CreateIOHandler(SOCKET socket, net::Reactor* reactor) {
    return new ZNetCommHandler(socket, reactor);
  }

private:
  friend class Singleton<ZNetCommHandlerFactory>;
  friend struct DefaultSingletonTraits<ZNetCommHandlerFactory>;

  ZNetCommHandlerFactory() {}
  virtual ~ZNetCommHandlerFactory() {}

  DISALLOW_COPY_AND_ASSIGN(ZNetCommHandlerFactory);
};

#endif

