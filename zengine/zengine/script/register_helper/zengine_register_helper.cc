// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/register_helper/zengine_register_helper.h"

// #include "luabind/luabind.hpp"
// #include "luabind/out_value_policy.hpp"
// #include "luabind/iterator_policy.hpp"
#include "scriptengine/luatinker/lua_tinker.h"

//#include "zengine/client_comm_handler.h"
//#include "zengine/zengine_daemon.h"
#include "zengine/zengine_context.h"
// #include "zengine/config_file.h"
#include "zengine/zengine_root.h"

namespace {

static const std::string kZEngineModuleName("mod_zengine");

bool CreateZEngineThread(const std::string& instance_name) {
  return zengine::Root::GetInstance()->CreateZEngineThread(instance_name);
}

void Luabind_ZEngine_Register(lua_State* L) {
  // io_handler
//   luabind::module(L) [
//     luabind::class_<zengine::CStringVector>("CStringVector")
//       .def_readwrite("cstring_vector", &zengine::CStringVector::cstring_vector, luabind::return_stl_iterator)
//       .def("size", &zengine::CStringVector::size),
// 
//     luabind::class_<zengine::StringVector>("StringVector")
//       .def_readwrite("string_vector", &zengine::StringVector::string_vector, luabind::return_stl_iterator)
//       .def("size", &zengine::StringVector::size),
// 
//     luabind::class_<ConfigFile>("ConfigFile")
//       .scope [
//         luabind::def("GetInstance", &ConfigFile::GetInstance)
//       ]
//       .def("GetString", &ConfigFile::GetString)
//       .def("GetStringList", &ConfigFile::GetStringList)
//       .def("GetCStringList", &ConfigFile::GetCStringList)
//       .def("GetSectionKey", &ConfigFile::GetSectionKey)
//       .def("GetSectionName", &ConfigFile::GetSectionName),

    lua_tinker::class_add<ZEngineContext>(L, "ZEngineContext");

    lua_tinker::class_def<ZEngineContext>(L, "PostTaskByInstanceName", &ZEngineContext::PostTaskByInstanceName);
    lua_tinker::class_def<ZEngineContext>(L, "PostTaskByInstanceID", &ZEngineContext::PostTaskByInstanceID);
    lua_tinker::class_def<ZEngineContext>(L, "PostTaskToIOThread", &ZEngineContext::PostTaskToIOThread);
    lua_tinker::class_def<ZEngineContext>(L, "PostTaskToMainThread", &ZEngineContext::PostTaskToMainThread);
    lua_tinker::class_def<ZEngineContext>(L, "PostTask", &ZEngineContext::PostTask);
    lua_tinker::class_def<ZEngineContext>(L, "SetTimer", &ZEngineContext::SetTimer);
    lua_tinker::class_def<ZEngineContext>(L, "KillTimer", &ZEngineContext::KillTimer);
    lua_tinker::class_def<ZEngineContext>(L, "context_type", &ZEngineContext::context_type);
    lua_tinker::class_def<ZEngineContext>(L, "instance_name", &ZEngineContext::instance_name);
    lua_tinker::class_def<ZEngineContext>(L, "context_type", &ZEngineContext::instance_id);

    lua_tinker::def(L, "CreateZEngineThread", &CreateZEngineThread);
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void ZEngine_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(kZEngineModuleName, &Luabind_ZEngine_Register);
}

}
