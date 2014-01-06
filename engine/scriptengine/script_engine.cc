// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "scriptengine/script_engine.h"

#include <list>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <luabind/iterator_policy.hpp>

#include "base/logging.h"

//#include "scriptengine/base_luabind_pb.h"
//#include "scriptengine/luabind_engine_register.h"

// #include "scriptengine/luabind_register_manager.h"

//int luaopen_zmq(lua_State *L);

void ScriptEngine::Initialize() {
  // 确保只会初始化一次
  if (!lua_state_) {
    lua_state_ = lua_open(); // 创建Lua状态
    if (!lua_state_) {
      LOG(ERROR) << "ScriptEngine::Init() lua_open failed!";
      return;
    }
    luaL_openlibs(lua_state_); // 为该Lua状态打开所有Lua库
    luabind::open(lua_state_); // 为该Lua状态打开luabind库
    //luaopen_zmq(lua_state_);

    //lua_interface_.Init(lua_state_);
  }
}

ScriptEngine::~ScriptEngine() {
  Destroy();
}

void ScriptEngine::Refresh() {
  Destroy();
  Initialize();
}

void ScriptEngine::Destroy() {
  if (lua_state_) {
    lua_close(lua_state_);
    lua_state_ = NULL;
  }
}

void ScriptEngine::Load(const std::string& lua_file) {
  CHECK(lua_state_ != NULL);

  if(boost::filesystem::exists(lua_file)) {
    if (luaL_dofile(lua_state_, lua_file.c_str())) {
      LOG(ERROR) << "load " << lua_file << " error: " << lua_tostring(lua_state_, -1);
    }
  }
}

//void ScriptEngine::RegisterDefault() {
//  // 注册BaseLuabindPB
//  //BaseLuabindPB_Register(lua_state_);
//  Luabind_Engine_Register(lua_state_);
//}
