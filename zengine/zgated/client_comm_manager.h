// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZGATED_CLIENT_COMM_MANAGER_H_
#define ZGATED_CLIENT_COMM_MANAGER_H_
#pragma once

#include "base/memory/singleton.h"
#include "base/memory/ref_counted.h"

#include "net/engine/tcp_acceptor.h"

namespace net {
class Reactor;
}
class ClientCommHandler;
class ClientCommManager{
public:
	static ClientCommManager* GetInstance() {
		return Singleton<ClientCommManager>::get();
	}

	bool Initialize(net::NetEngineManager* engine_manager, const std::string& addr, const std::string& port);
  bool SendSessionData(uint64 session_id, const char* io_buffer, uint32 io_buffer_len);
  net::Reactor* LookupReactorBySessionID(uint64 session_id);

private:
	scoped_refptr<net::TCPAcceptor> acc_;
  net::NetEngineManager* net_engine_manager_;

	//////////////////////////////////////////////////////////////////////////
	friend class Singleton<ClientCommManager>;
	friend struct DefaultSingletonTraits<ClientCommManager>;

	ClientCommManager()
    : net_engine_manager_(NULL) {
  }

	~ClientCommManager() {}

	DISALLOW_COPY_AND_ASSIGN(ClientCommManager);
};

#endif

