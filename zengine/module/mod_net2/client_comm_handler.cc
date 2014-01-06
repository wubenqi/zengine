// Copyright 2010, www.lelewan.com
// All rights reserved
//
// Author: wubenqi<wubenqi@caitong.net>, 2013-01-09
//

#include "zengine/client_comm_handler.h"

#include "base/bind.h"
#include "base/md5.h"
#include "base/stringprintf.h"

#include "net/base/io_buffer.h"
#include "net/engine/reactor.h"

#include "zengine/script/script_manager.h"
#include "zengine/zengine_context.h"

//inline const char* CommHandlerStateToString(int conn_state) {
//  const char* state_string = "kCommHandlerStateInvalid";
//
//  switch (conn_state){
//  case ClientCommHandler::kCommHandlerStateConnected:
//    state_string = "kCommHandlerStateConnected";
//    break;
//  case ClientCommHandler::kCommHandlerStateWorking:
//    state_string = "kCommHandlerStateWorking";
//    break;
//  case ClientCommHandler::kCommHandlerStateClosed:
//    state_string = "kCommHandlerStateClosed";
//    break;
//  default:
//    break;
//  }
//  return state_string;
//}

ClientCommHandler::ClientCommHandler(SOCKET socket, net::Reactor* reactor) 
  : net::HammerIOHandler(socket, reactor)  
  , context_(NULL) {
}

ClientCommHandler::~ClientCommHandler() {
  Destroy();
}

void ClientCommHandler::Destroy() {
}

int ClientCommHandler::OnNewConnection() {
  log_prefix_ = base::StringPrintf("[%u@%u]>> ", GetLowInt32ByInt64(GetSessionID()), GetReactorID());
  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " connected";

  context_ = reinterpret_cast<ZEngineContext*>(GetReactor()->GetUserData());
  DCHECK(context_);
  //script_manager_ = zengine_daemon_->GetIOScriptManager(GetReactorID());

  //DCHECK(script_manager_);
  context_->script_manager()->ExeScript_OnNewConnection(this);

  return 0;
}

int ClientCommHandler::OnDataReceived(const PacketPtr& packet) {
  DCHECK(context_);
  context_->script_manager()->ExeScript_OnDataReceived(this, packet->GetCmdType(), reinterpret_cast<char*>(packet->GetBodyData()), packet->GetBodyLength());
  return 0;
}

int ClientCommHandler::OnConnectionClosed() {
  LOG(INFO) << log_prefix_ << "client_comm_handler from " << GetAddressString() << " closed";
  DCHECK(context_);
  context_->script_manager()->ExeScript_OnConnectionClosed(this);
  return 0;
}
