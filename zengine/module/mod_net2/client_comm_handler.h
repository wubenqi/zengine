// Copyright 2010, www.lelewan.com
// All rights reserved
//
// Author: wubenqi<wubenqi@caitong.net>, 2013-01-09
//

#ifndef ZENGINE_CLIENT_COMM_HANDLER_H_
#define ZENGINE_CLIENT_COMM_HANDLER_H_
#pragma once

#include "base/memory/singleton.h"

#include "net/hammer/hammer_io_handler.h"
#include "net/engine/io_handler_factory.h"

//class ZEngineDaemon;
//class ScriptManager;
class ZEngineContext;
class ClientCommHandler :
  public net::HammerIOHandler {
public:
  ClientCommHandler(SOCKET socket, net::Reactor* reactor);
  virtual ~ClientCommHandler();
  void Destroy();

  //////////////////////////////////////////////////////////////////////////
  virtual int OnNewConnection();
  virtual int OnDataReceived(const PacketPtr& packet);
  virtual int OnConnectionClosed();

  ZEngineContext* GetContext() {
    return context_;
  }

private:
  //int conn_state_;
  std::string log_prefix_;
  ZEngineContext* context_;
  //ZEngineDaemon* zengine_daemon_;
  //ScriptManager* script_manager_;
};

class ClientCommHandlerFactory :
  public net::IOHandlerFactory {
public:
  static ClientCommHandlerFactory* GetInstance() {
    return Singleton<ClientCommHandlerFactory>::get();
  }

  virtual net::IOHandler* CreateIOHandler(SOCKET socket, net::Reactor* reactor) {
    return new ClientCommHandler(socket, reactor);
  }

private:
  friend class Singleton<ClientCommHandlerFactory>;
  friend struct DefaultSingletonTraits<ClientCommHandlerFactory>;

  ClientCommHandlerFactory() {}
  virtual ~ClientCommHandlerFactory() {}

  DISALLOW_COPY_AND_ASSIGN(ClientCommHandlerFactory);
};

#endif

