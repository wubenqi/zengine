// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_console/register_helper/console_register_helper.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/raw_policy.hpp"

#include "mod_console/module_console_dll.h"

namespace {

void Luabind_Console_Register(lua_State* L) {
  //luabind::module(L) [
  //  luabind::def("GetDBConnPoolMgrByInstanceName", &GetDBConnPoolMgrByInstanceName),
  //  luabind::def("GetDBConnPoolMgr", &GetDBConnPoolMgr)
  //];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void Console_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Console(), &Luabind_Console_Register);
}

}

