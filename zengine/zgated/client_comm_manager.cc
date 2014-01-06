// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zgated/client_comm_manager.h"

#include "net/engine/net_engine_manager.h"

#include "zgated/client_comm_handler.h"

bool ClientCommManager::Initialize(net::NetEngineManager* engine_manager, const std::string& addr, const std::string& port) {
  net_engine_manager_ = engine_manager;
	acc_ = new net::TCPAcceptor(engine_manager, ClientCommHandlerFactory::GetInstance());
	DCHECK(acc_.get());
	return acc_->Create(addr, port, true);
}

bool ClientCommManager::SendSessionData(uint64 session_id, const char* io_buffer, uint32 io_buffer_len) {
  DCHECK(net_engine_manager_);

  return net_engine_manager_->SendSessionData(session_id, io_buffer, io_buffer_len);
}

net::Reactor* ClientCommManager::LookupReactorBySessionID(uint64 session_id) {
  DCHECK(net_engine_manager_);

  return net_engine_manager_->LookupReactorBySessionID(session_id);
}
