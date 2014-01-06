// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//


#ifndef ZGATED_SERVER_COMM_MANAGER_H_
#define ZGATED_SERVER_COMM_MANAGER_H_
#pragma once

#include "base/memory/singleton.h"
#include "base/memory/ref_counted.h"

#include "base/synchronization/lock.h"
#include "base2/scoped_lock.h"

#include "net/engine/tcp_acceptor.h"

namespace net {
class Reactor;
}
class ServerCommHandler;
class ServerCommManager{
public:
	static ServerCommManager* GetInstance() {
		return Singleton<ServerCommManager>::get();
	}

	bool Initialize(net::NetEngineManager* engine_manager, const std::string& addr, const std::string& port);

  int SendDataToServer(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);
  int SendDataToServer(int conn_type, uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);

  int SendDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);
  int PostTaskDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len);

  void OnNewConnection(int conn_type, uint64 session_id);
  void OnConnectionClosed(int conn_type);

private:
	scoped_refptr<net::TCPAcceptor> acc_;
  net::NetEngineManager* net_engine_manager_;

  base::Lock zlogin_session_lock_;
  uint64 zlogin_session_id_;

  base::Lock zgame_session_lock_;
  uint64 zgame_session_id_;

	//////////////////////////////////////////////////////////////////////////
	friend class Singleton<ServerCommManager>;
	friend struct DefaultSingletonTraits<ServerCommManager>;

	ServerCommManager()
    : net_engine_manager_(NULL)
    , zlogin_session_id_(0)
    , zgame_session_id_(0) {
  }

	~ServerCommManager() {}

	DISALLOW_COPY_AND_ASSIGN(ServerCommManager);
};

#endif

