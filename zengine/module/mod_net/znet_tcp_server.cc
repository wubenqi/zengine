// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net/znet_tcp_server.h"

#include "base/logging.h"

#include "zengine/zengine_context_manager.h"
#include "zengine/script/script_manager.h"
#include "zengine/zengine_context.h"

namespace zengine {

bool ZNetTCPServer::Create(const CommNodeInfo& comm_node_info) {
  if (server_.get() == NULL) {
    ZEngineContext* ctx = ZEngineContextManager::GetInstance()->LookupMainContext();
    CHECK(ctx);
    script_ = ctx->script_manager()->GetScriptEngine();

    znet_handler_context_.reset(new ZNetHandlerContext(ctx, comm_node_info.net_type, comm_node_info.instance_name));
    server_.reset(new net::TCPServer(ctx->message_loop(), this));
    return server_->Create(comm_node_info.addr, comm_node_info.port, true);
  } else {
    LOG(ERROR) << "ZNetTCPServer is created!!!!";
    return false;
  }
  return true;
}

int ZNetTCPServer::OnNewConnection(const net::IOHandlerPtr& ih) {
  ih->SetIOContext(znet_handler_context_.get());

  int result = script_->CallFunction<int, net::IOHandler*>("OnNewConnection", ih.get());
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnNewConnection() error, error_code = " << result;
  }
  return 0;
}

int ZNetTCPServer::OnDataReceived(const net::IOHandlerPtr& ih, net::IOBuffer* data, base::Time receive_time) {
  DCHECK(script_);

  base::StringPiece buf = data->ToStringPiece();
  data->RetrieveAll();

  int result = script_->CallFunction<int, net::IOHandler*, base::StringPiece>("OnDataReceived", ih.get(), buf);
  data->RetrieveAll();
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnDataReceived() error, error_code = " << result;
  }
  return 0;
}

int ZNetTCPServer::OnConnectionClosed(const net::IOHandlerPtr& ih) {

  int result = script_->CallFunction<int, net::IOHandler*>("OnConnectionClosed", ih.get());
  if (result!=0) {
    LOG(ERROR) << "ERROR: In main.lua, Execute OnConnectionClosed() error, error_code = " << result;
  }

  ih->ReleaseIOContext();
  return 0;
}


}
