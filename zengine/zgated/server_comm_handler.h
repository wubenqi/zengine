// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZGATED_SERVER_COMM_HANDLER_H_
#define ZGATED_SERVER_COMM_HANDLER_H_
#pragma once

#include "base/memory/singleton.h"
#include "base/timer.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/platform_thread.h"

#include "net/engine/tcp_acceptor.h"
#include "net/hammer/hammer_io_handler.h"

class ServerCommHandler :
  public net::HammerIOHandler {
public:
  enum CommHandkerType {
    kCommHandkerType_Invalid = 0,
    kCommHandkerType_ZGamed = 1,
    kCommHandkerType_ZLogind = 2,
  };

  enum CommHandlerState {
    kCommHandlerStateInvalid = 0,	  // 非法
    kCommHandlerStateConnected = 1,	// 已经连接
    kCommHandlerStateNormal = 2,	  // 正常工作
    kCommHandlerStateClosed = 3,	  // 已经关闭
  };

  ServerCommHandler(SOCKET socket, net::Reactor* reactor);
  virtual ~ServerCommHandler();
  void Destroy();

  //////////////////////////////////////////////////////////////////////////
  virtual int OnNewConnection();
  virtual int OnDataReceived(const PacketPtr& packet);
  virtual int OnConnectionClosed();

  //int SendDataToServer(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);
  int SendDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);
  int PostDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);

protected:
  //////////////////////////////////////////////////////////////////////////
  int conn_state_;
  //std::string client_instance_name_;
  CommHandkerType conn_type_;
  //base::Lock lock_;
};

class ServerCommHandlerFactory :
  public net::IOHandlerFactory {
public:
  static ServerCommHandlerFactory* GetInstance() {
    return Singleton<ServerCommHandlerFactory>::get();
  }

  virtual net::IOHandler* CreateIOHandler(SOCKET socket, net::Reactor* reactor) {
    return new ServerCommHandler(socket, reactor);
  }

private:
  friend class Singleton<ServerCommHandlerFactory>;
  friend struct DefaultSingletonTraits<ServerCommHandlerFactory>;

  ServerCommHandlerFactory() {}
  virtual ~ServerCommHandlerFactory() {}

  DISALLOW_COPY_AND_ASSIGN(ServerCommHandlerFactory);
};

#endif

