// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

struct lua_State;

{{#ONE_SET_HANDLER2}}void LuabindPB_{{PROTO_FILE_NAME2}}_RegisterAll(lua_State* L);
{{/ONE_SET_HANDLER2}}

void LuabindPB_RegisterAll(lua_State* L) {
{{#ONE_SET_HANDLER}}  LuabindPB_{{PROTO_FILE_NAME}}_RegisterAll(L);
{{/ONE_SET_HANDLER}}
}

#if defined(WIN32)
#include <windows.h>
#else
#define WINAPI
#endif

extern "C" void WINAPI LuabindPBModule_Register(lua_State* L) {
  LuabindPB_RegisterAll(L);
}
