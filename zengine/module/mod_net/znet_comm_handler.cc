// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net/znet_comm_handler.h"

//#include "base/bind.h"
//#include "base/md5.h"
#include "base/stringprintf.h"
#include "base2/io_buffer2.h"

#include "zcommon/config_util.h"
//#include "net/base/io_buffer.h"
#include "net/engine/reactor.h"

#include "zengine/script/script_manager.h"
#include "zengine/zengine_context.h"

//inline const char* CommHandlerStateToString(int conn_state) {
//  const char* state_string = "kCommHandlerStateInvalid";
//
//  switch (conn_state){
//  case ZNetCommHandler::kCommHandlerStateConnected:
//    state_string = "kCommHandlerStateConnected";
//    break;
//  case ZNetCommHandler::kCommHandlerStateWorking:
//    state_string = "kCommHandlerStateWorking";
//    break;
//  case ZNetCommHandler::kCommHandlerStateClosed:
//    state_string = "kCommHandlerStateClosed";
//    break;
//  default:
//    break;
//  }
//  return state_string;
//}

ZNetCommHandler::ZNetCommHandler(SOCKET socket, net::Reactor* reactor) 
  : net::HammerIOHandler(socket, reactor)  
  , context_(NULL)
  , conn_state_(0) {
}

ZNetCommHandler::~ZNetCommHandler() {
  Destroy();
}

void ZNetCommHandler::Destroy() {
}

void ZNetCommHandler::SetUserData(void* user_data) {
  comm_node_info_ = reinterpret_cast<CommNodeInfo*>(user_data);
}


int ZNetCommHandler::OnNewConnection() {
  string_session_id_ = base::StringPrintf("%u@%u", GetLowInt32ByInt64(GetSessionID()), GetReactorID());
  log_prefix_ = base::StringPrintf("[%u@%u]>> ", GetLowInt32ByInt64(GetSessionID()), GetReactorID());
  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " connected";

  context_ = reinterpret_cast<ZEngineContext*>(GetReactor()->GetUserData());
  DCHECK(context_);
  DCHECK(context_->script_manager());
  int result = context_->script_manager()->script_engine().CallFunction<int, ZNetCommHandler*>("OnNewConnection", this);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnNewConnection() error, error_code = " << result;
  }

  return 0;
}

int ZNetCommHandler::OnDataReceived(const PacketPtr& packet) {
  if (packet->GetBodyLength()<sizeof(uint64)) {
    LOG(ERROR) << log_prefix_ << "ZNetCommHandler::OnDataReceived - recv a invalid packet, data_len < sizeof(uint64) by message_type " << packet->GetCmdType();
    return 0;
  }
  
  DCHECK(context_);
  DCHECK(context_->script_manager());

  uint16 message_type = packet->GetCmdType();
  //uint64 client_id = 0;
  //memcpy(&client_id, packet->GetBodyData(), sizeof(uint64));
  IOBuffer2 io_buffer(reinterpret_cast<const char*>(packet->GetBodyData()), packet->GetBodyLength());
  //
  //  IOBuffer2 io_buffer(data, data_len);
  //  int result = script_engine_.CallFunction("OnOnDataReceived", ih, message_type, io_buffer);
  //  if (result!=0) {
  //    LOG(ERROR) << "ERROR: In main.lua, Execute OnDataReceived() error, error_code = " << result;
  //  }
  //
  int result = context_->script_manager()->script_engine().CallFunction<int, ZNetCommHandler*, uint16, IOBuffer2*>("OnDataReceived", this, message_type, &io_buffer);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnDataReceived() error, error_code = " << result;
  }
  return 0;
}

int ZNetCommHandler::OnConnectionClosed() {
  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " closed";
  DCHECK(context_);
  DCHECK(context_->script_manager());

  int result = context_->script_manager()->script_engine().CallFunction<int, ZNetCommHandler*>("OnConnectionClosed", this);
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnConnectionClosed() error, error_code = " << result;
  }
  return 0;
}

const std::string& ZNetCommHandler::GetInstanceName() const {
  return comm_node_info_->instance_name;
}

const std::string& ZNetCommHandler::GetNetType()  const {
  return comm_node_info_->net_type;
}
