// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "luabind_pb/luabind_pb_module.h"

void LuabindPB_RegisterAll(lua_State* L);

extern "C" void WINAPI LuabindPBModule_Register(lua_State* L) {
  LuabindPB_RegisterAll(L);
}
