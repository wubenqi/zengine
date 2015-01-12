// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_net/register_helper/net_register_helper.h"

#include "base/basictypes.h"

#include "net/base/io_buffer.h"
#include "net/engine/io_handler.h"
#include "scriptengine/luatinker/lua_tinker.h"

#include "zengine/zengine_context.h"
#include "mod_net/module_net_dll.h"

namespace {

void Luabind_Net_Register(lua_State* L) {
  lua_tinker::class_add<base::StringPiece>(L, "StringPiece");

  lua_tinker::class_add<net::IOBuffer>(L, "IOBuffer");
  lua_tinker::class_def<net::IOBuffer>(L, "ReadableBytes", &net::IOBuffer::ReadableBytes);
  lua_tinker::class_def<net::IOBuffer>(L, "WritableBytes", &net::IOBuffer::WritableBytes);
  lua_tinker::class_def<net::IOBuffer>(L, "RetrieveAllAsString", &net::IOBuffer::RetrieveAllAsString);

  lua_tinker::class_add<net::IOHandler>(L, "IOHandler");
  lua_tinker::class_def<net::IOHandler>(L, "GetAddressString", &net::IOHandler::GetAddressString);
  lua_tinker::class_def<net::IOHandler>(L, "io_handler_id", &net::IOHandler::io_handler_id);
  lua_tinker::class_def<net::IOHandler>(L, "SendData", (bool(net::IOHandler::*)(net::IOBuffer*))&net::IOHandler::SendData);
  lua_tinker::class_def<net::IOHandler>(L, "SendString", &net::IOHandler::SendStringPiece);
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void Net_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Net(), &Luabind_Net_Register);
}

}
