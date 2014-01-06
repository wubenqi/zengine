// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zgated/client_comm_handler.h"

#include "base/file_util.h"
#include "base/bind.h"
#include "base/md5.h"
#include "base/stringprintf.h"

#include "net/engine/reactor.h"
#include "base2/scoped_lock.h"

#include "proto/cc/common/const_message_types.pb.h"
#include "proto/cc/server/sc_auth_challenge.pb.h"
#include "proto/cc/server/sc_auth_proof.pb.h"

#include "zgated/server_comm_manager.h"
#include "zgated/server_comm_handler.h"

inline const char* CommHandlerStateToString(int conn_state) {
  const char* state_string = "kCommHandlerStateInvalid";

  switch (conn_state){
  case ClientCommHandler::kCommHandlerStateConnected:
    state_string = "kCommHandlerStateConnected";
    break;
  case ClientCommHandler::kCommHandlerStateNormal:
    state_string = "kCommHandlerStateNormal";
    break;
  case ClientCommHandler::kCommHandlerStateClosing:
    state_string = "kCommHandlerStateClosing";
    break;
  case ClientCommHandler::kCommHandlerStateClosed:
    state_string = "kCommHandlerStateClosed";
    break;
  default:
    break;
  }
  return state_string;
}

const scoped_refptr<ClientCommHandler>& GetEmptyClientCommHandler() {
  static scoped_refptr<ClientCommHandler> kEmptyClientHandler;
  return kEmptyClientHandler;
}

ClientCommHandler::ClientCommHandler(SOCKET socket, net::Reactor* reactor) 
  : net::HammerIOHandler(socket, reactor)
  , conn_state_(kCommHandlerStateInvalid)
  , thread_id_(base::kInvalidThreadId)
  , server_comm_handler_(NULL) {

}
ClientCommHandler::~ClientCommHandler() {
}

void ClientCommHandler::Destroy() {
}

int ClientCommHandler::OnNewConnection() {
  log_prefix_ = base::StringPrintf("[%u@%u]>> ", GetLowInt32ByInt64(GetSessionID()), GetReactorID());

  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " connected";
  conn_state_ = kCommHandlerStateConnected;

  ServerCommManager::GetInstance()->SendDataToServer(ServerCommHandler::kCommHandkerType_ZLogind, GetSessionID(), hammer::message::MESSAGE_CS_SERVER_SESSION_NEW, GetAddressString().c_str(), GetAddressString().length());
  // 等待连接消息
  return 0;
}

int ClientCommHandler::OnDataReceived(const PacketPtr& packet) {
  // 1. 玩家登录处理
  // 2. 转发
  // 3. 

  LOG(INFO) << log_prefix_ << "recv a packet, message_type = " << packet->GetCmdType() << ", data_len = " << packet->GetBodyLength() << ", from " << GetAddressString();
  int result = -1;

  switch (conn_state_) {
  case kCommHandlerStateConnected:
    switch (packet->GetCmdType()) {
    case hammer::message::MESSAGE_CS_AUTH_CHALLENGE:
      //发送给服务器
      result = ServerCommManager::GetInstance()->SendDataToServer(GetSessionID(), packet->GetCmdType(), reinterpret_cast<char*>(packet->GetBodyData()), packet->GetBodyLength());
      break;
    case hammer::message::MESSAGE_CS_AUTH_PROOF:
      //发送给服务器
      result = ServerCommManager::GetInstance()->SendDataToServer(GetSessionID(), packet->GetCmdType(), reinterpret_cast<char*>(packet->GetBodyData()), packet->GetBodyLength());
      //conn_state_ = kCommHandlerStateNormal;
    	break;
    default:
      LOG(ERROR) << log_prefix_ << "conn state is kCommHandlerStateConnected, but recv a message_type[" << packet->GetCmdType() << "] message, drop the message";
      break;
    }
  	break;
  case kCommHandlerStateNormal:
    switch (packet->GetCmdType()) {
    case hammer::message::MESSAGE_CS_AUTH_CHALLENGE:
    case hammer::message::MESSAGE_CS_AUTH_PROOF:
      LOG(ERROR) << log_prefix_ << "conn state is kCommHandlerStateConnected, but recv a message_type[" << packet->GetCmdType() << "] message, drop the message";
      break;
    default:
      //发送给服务器
      result = ServerCommManager::GetInstance()->SendDataToServer(GetSessionID(), packet->GetCmdType(), reinterpret_cast<char*>(packet->GetBodyData()), packet->GetBodyLength());
      break;
    }
    break;
  default:
    LOG(ERROR) << log_prefix_ << "conn state is " << CommHandlerStateToString(conn_state_)
      << ", recv message_type[" << packet->GetCmdType() 
      << "] message, drop the message";
    break;
  }
  return 0;
}

int ClientCommHandler::OnConnectionClosed() {
  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " closed";

  if (conn_state_ == kCommHandlerStateNormal) {
    ServerCommManager::GetInstance()->SendDataToServer(ServerCommHandler::kCommHandkerType_ZGamed, GetSessionID(), hammer::message::MESSAGE_CS_SERVER_SESSION_CLOSE, NULL, 0);
  } else if (conn_state_ == kCommHandlerStateConnected) {
    ServerCommManager::GetInstance()->SendDataToServer(ServerCommHandler::kCommHandkerType_ZLogind, GetSessionID(), hammer::message::MESSAGE_CS_SERVER_SESSION_CLOSE, NULL, 0);
  }
  conn_state_ = kCommHandlerStateClosed;
  return 0;
}

void ClientCommHandler::OnTaskDataReceived(const char* data, uint32 data_len) {
  if (data_len<net::HammerPacketHeader::kPacketHeaderSize) {
    LOG(ERROR) << "recv a invalid task data, data_len = " << data_len;
    return;
  }
  
  uint16 cmd_type = 0;
  uint32 body_len = 0;

  if(-1==net::HammerPacketHeader::ParsePacketHeader(reinterpret_cast<const uint8*>(data), &cmd_type, &body_len)) {
    LOG(ERROR) << "recv a invalid task data, parse data error, data_len = " << data_len;
    Close();
    return;
  }

  if (cmd_type == hammer::message::MESSAGE_SC_AUTH_CHALLENGE) {
    hammer::message::SCAuthChallenge sc_auth_challenge;
    if(!sc_auth_challenge.ParseFromArray(data+net::HammerPacketHeader::kPacketHeaderSize, body_len)) {
      LOG(ERROR) << "parse sc_auth_challenge message error";
      Close();
      return;
    }
    if (sc_auth_challenge.ret_code()!=0) {
      Close();
      return;
    }
    SendInternal(data, data_len);
  } else if (cmd_type == hammer::message::MESSAGE_SC_AUTH_PROOF) {
    hammer::message::SCAuthProof sc_auth_proof;
    if(!sc_auth_proof.ParseFromArray(data+net::HammerPacketHeader::kPacketHeaderSize, body_len)) {
      LOG(ERROR) << "parse sc_auth_challenge message error";
      Close();
      return;
    }
    if (sc_auth_proof.ret_code()!=0) {
      Close();
      return;
    }
    ServerCommManager::GetInstance()->SendDataToServer(ServerCommHandler::kCommHandkerType_ZLogind, GetSessionID(), hammer::message::MESSAGE_CS_SERVER_SESSION_CLOSE, NULL, 0);
    conn_state_ = kCommHandlerStateNormal;
    ServerCommManager::GetInstance()->SendDataToServer(ServerCommHandler::kCommHandkerType_ZGamed, GetSessionID(), hammer::message::MESSAGE_CS_SERVER_SESSION_NEW, GetAddressString().c_str(), GetAddressString().length());
    SendInternal(data, data_len);
  } else {
    LOG(ERROR) << "recv a invalid task data, message_type = " << cmd_type;
    return;
  }
  
}
