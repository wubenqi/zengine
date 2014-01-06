// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//


#include "zgated/server_comm_manager.h"

#include "net/engine/net_engine_manager.h"

#include "zgated/server_comm_handler.h"

#include "zcommon/message_util.h"
#include "proto/cc/common/const_message_types.pb.h"

bool ServerCommManager::Initialize(net::NetEngineManager* engine_manager, const std::string& addr, const std::string& port) {
  net_engine_manager_ = engine_manager;
	acc_ = new net::TCPAcceptor(engine_manager, ServerCommHandlerFactory::GetInstance());
	DCHECK(acc_.get());
  return acc_->Create(addr, port, true);
}

//bool ServerCommManager::SendSessionData(uint64 session_id, const char* io_buffer, uint32 io_buffer_len) {
//  DCHECK(net_engine_manager_);
//
//  return net_engine_manager_->SendSessionData(session_id, io_buffer, io_buffer_len);
//}
//
//net::Reactor* ServerCommManager::LookupReactorBySessionID(uint64 session_id) {
//  DCHECK(net_engine_manager_);
//
//  return net_engine_manager_->LookupReactorBySessionID(session_id);
//}

static inline bool IsLoginCmdType(uint16 cmd_type) {
  if (cmd_type == hammer::message::MESSAGE_CS_AUTH_CHALLENGE || cmd_type == hammer::message::MESSAGE_CS_AUTH_PROOF) {
    return true;
  }
  return false;
}

int ServerCommManager::SendDataToServer(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  DCHECK(net_engine_manager_);

  uint64 server_session_id = 0;
  if (IsLoginCmdType(cmd_type)) {
    base::ScopedLock scoped_lock(zlogin_session_lock_);
    server_session_id = zlogin_session_id_;
  } else {
    base::ScopedLock scoped_lock(zgame_session_lock_);
    server_session_id = zgame_session_id_;
  }

  if (server_session_id == 0) {
    return -1;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, cmd_type, data, data_len);
  //// 在数据字段最前面添加session_id字段
  //net::IOBufferPtr io_buffer = new net::IOBuffer(net::HammerPacketHeader::kPacketHeaderSize+sizeof(session_id)+data_len);
  //net::HammerPacketHeader::GeneratePacketHeader(reinterpret_cast<uint8*>(io_buffer->data()), cmd_type, data_len+sizeof(session_id));
  //memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize, &session_id, sizeof(session_id));
  //memcpy(io_buffer->data()+net::HammerPacketHeader::kPacketHeaderSize+sizeof(session_id), data, data_len);
  
  net_engine_manager_->SendSessionData(server_session_id, io_buffer);

  return 0;
}

int ServerCommManager::SendDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  DCHECK(net_engine_manager_);
  //if (server_session_id_ == 0) {
  //  return -1;
  //}

  net::IOBufferPtr io_buffer = MakeIOBuffer(cmd_type, data, data_len);
  net_engine_manager_->SendSessionData(session_id, io_buffer);

  return 0;
}

int ServerCommManager::PostTaskDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  DCHECK(net_engine_manager_);

  net::IOBufferPtr io_buffer = MakeIOBuffer(cmd_type, data, data_len);
  net_engine_manager_->PostTaskData(session_id, io_buffer);

  return 0;
}

int ServerCommManager::SendDataToServer(int conn_type, uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  DCHECK(net_engine_manager_);

  uint64 server_session_id = 0;
  if (conn_type == ServerCommHandler::kCommHandkerType_ZLogind) {
    base::ScopedLock scoped_lock(zlogin_session_lock_);
    server_session_id = zlogin_session_id_;
  } else if (conn_type == ServerCommHandler::kCommHandkerType_ZGamed) {
    base::ScopedLock scoped_lock(zgame_session_lock_);
    server_session_id = zgame_session_id_;
  }

  if (server_session_id == 0) {
    return -1;
  }

  net::IOBufferPtr io_buffer = MakeIOBuffer(session_id, cmd_type, data, data_len);
  net_engine_manager_->SendSessionData(server_session_id, io_buffer);

  return 0;
}

void ServerCommManager::OnNewConnection(int conn_type, uint64 session_id) {
  if (conn_type == ServerCommHandler::kCommHandkerType_ZLogind) {
    base::ScopedLock scoped_lock(zlogin_session_lock_);
    zlogin_session_id_ = session_id;
  } else if (conn_type == ServerCommHandler::kCommHandkerType_ZGamed) {
    base::ScopedLock scoped_lock(zgame_session_lock_);
    zgame_session_id_ = session_id;
  }
}

void ServerCommManager::OnConnectionClosed(int conn_type) {
  if (conn_type == ServerCommHandler::kCommHandkerType_ZLogind) {
    base::ScopedLock scoped_lock(zlogin_session_lock_);
    zlogin_session_id_ = 0;
  } else if (conn_type == ServerCommHandler::kCommHandkerType_ZGamed) {
    base::ScopedLock scoped_lock(zgame_session_lock_);
    zgame_session_id_ = 0;
  }
}
