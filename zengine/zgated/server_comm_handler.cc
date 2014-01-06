// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zgated/server_comm_handler.h"

#include "base/file_util.h"
#include "base/bind.h"
#include "base/md5.h"

#include "net/engine/reactor.h"
#include "base2/scoped_lock.h"
#include "base2/io_buffer2.h"

#include "zcommon/io_handler_util.h"

#include "proto/cc/common/const_message_types.pb.h"
#include "proto/cc/server/cs_server_challeng_request.pb.h"
#include "proto/cc/server/sc_server_challeng_ack.pb.h"

#include "zgated/server_comm_manager.h"


inline const char* CommHandlerStateToString(int conn_state) {
  const char* state_string = "kCommHandlerStateInvalid";

  switch (conn_state){
  case ServerCommHandler::kCommHandlerStateConnected:
    state_string = "kCommHandlerStateConnected";
    break;
  case ServerCommHandler::kCommHandlerStateNormal:
    state_string = "kCommHandlerStateNormal";
    break;
  case ServerCommHandler::kCommHandlerStateClosed:
    state_string = "kCommHandlerStateClosed";
    break;
  default:
    break;
  }
  return state_string;
}

ServerCommHandler::ServerCommHandler(SOCKET socket, net::Reactor* reactor) 
  : net::HammerIOHandler(socket, reactor)
  , conn_state_(kCommHandlerStateInvalid)
  , conn_type_(kCommHandkerType_Invalid) {

}
ServerCommHandler::~ServerCommHandler() {
}

void ServerCommHandler::Destroy() {
}

int ServerCommHandler::OnNewConnection() {
  LOG(INFO) << "server_comm_handler from " << GetAddressString() << " connected";
  conn_state_ = kCommHandlerStateConnected;
  return 0;
}

int ServerCommHandler::OnDataReceived(const PacketPtr& packet) {
  //if (packet->GetBodyLength()<sizeof(uint64)) {
  //  LOG(ERROR) << "recv a invlaid packet, message_type = " << packet->GetCmdType()
  //    << ", data_len = " << packet->GetBodyLength();
  //  return 0;
  //}

  IOBuffer2 io_buffer(packet->GetBodyData(), packet->GetBodyLength());
  if (io_buffer.size()<sizeof(uint64)) {
    LOG(ERROR) << "recv a invlaid packet, message_type = " << packet->GetCmdType()
      << ", data_len = " << io_buffer.size();
    return 0;
  }
  uint64 client_session_id = 0;
  io_buffer.ReadUint64(&client_session_id);

  if (conn_state_ == kCommHandlerStateConnected) {
    if (packet->GetCmdType() == hammer::message::MESSAGE_CS_SERVER_CHALLENGE_REQUEST) {
      // 服务器连接
      hammer::message::CSServerChallengeRequest cs_server_challegne_ack;
      if (!cs_server_challegne_ack.ParseFromArray(io_buffer.read_ptr(), io_buffer.readable_size())) {
        LOG(ERROR) << "Parse CSServerChallengeRequest message data error, close conn";
        return -1;
      }

      if (cs_server_challegne_ack.name() == "zlogind") {
        conn_type_ = kCommHandkerType_ZLogind;
      } else if (cs_server_challegne_ack.name() == "zgamed") {
        conn_type_ = kCommHandkerType_ZGamed;
      } else {
        LOG(ERROR) << "conn name not zlogind or zgamed, is invalid";
        return -1;
      }
      conn_state_ = kCommHandlerStateNormal;
      ServerCommManager::GetInstance()->OnNewConnection(conn_type_, GetSessionID());

      hammer::message::SCServerChallengeAck sc_server_challenge_ack;
      sc_server_challenge_ack.set_ret_code(0);
      SendSessionMessageData(this, GetSessionID(), hammer::message::MESSAGE_SC_SERVER_CHALLENGE_ACK, sc_server_challenge_ack);
    } else {
      LOG(ERROR) << "recv a invlaid packet, message_type = " << packet->GetBodyLength()
        << ", data_len = " << packet->GetBodyLength();
      // 状态出错
    }
  } else if (conn_state_ == kCommHandlerStateNormal) {
    //uint8*  body = packet->GetBodyData();
    //uint64 client_session_id = 0;
    //memcpy(&client_session_id, body, sizeof(client_session_id));
    
    // 检查消息
    // SendSessionMessageData();
    if (conn_type_ == kCommHandkerType_ZLogind) {
      PostDataToClient(client_session_id, packet->GetCmdType(), io_buffer.read_ptr(), io_buffer.readable_size());
    } else {
      SendDataToClient(client_session_id, packet->GetCmdType(), io_buffer.read_ptr(), io_buffer.readable_size());
    }
    // 然后处理

    // 转发给客户端
  } else {
    // ERROR
  }
  
  return 0;
}

int ServerCommHandler::OnConnectionClosed() {
  LOG(INFO) << "server_comm_handler from " << GetAddressString() << " closed";
  conn_state_ = kCommHandlerStateClosed;
  ServerCommManager::GetInstance()->OnConnectionClosed(conn_type_);
  conn_type_ = kCommHandkerType_Invalid;
  return 0;
}


//int ServerCommHandler::SendDataToServer(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
//  return 0;
//}

int ServerCommHandler::SendDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  return ServerCommManager::GetInstance()->SendDataToClient(session_id, cmd_type, data, data_len);
}

int ServerCommHandler::PostDataToClient(uint64 session_id, uint16 cmd_type, const char* data, uint32 data_len) {
  return ServerCommManager::GetInstance()->PostTaskDataToClient(session_id, cmd_type, data, data_len);
}
