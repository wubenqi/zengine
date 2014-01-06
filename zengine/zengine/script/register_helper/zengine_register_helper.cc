// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/script/register_helper/zengine_register_helper.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/iterator_policy.hpp"

//#include "zengine/client_comm_handler.h"
//#include "zengine/zengine_daemon.h"
#include "zengine/zengine_context.h"
#include "zengine/config_file.h"
#include "zengine/zengine_root.h"

namespace {

static const std::string kZEngineModuleName("mod_zengine");

bool CreateZEngineThread(const std::string& instance_name) {
  return zengine::Root::GetInstance()->CreateZEngineThread(instance_name);
}

void Luabind_ZEngine_Register(lua_State* L) {
  // io_handler
  luabind::module(L) [
    luabind::class_<zengine::CStringVector>("CStringVector")
      .def_readwrite("cstring_vector", &zengine::CStringVector::cstring_vector, luabind::return_stl_iterator)
      .def("size", &zengine::CStringVector::size),

    luabind::class_<zengine::StringVector>("StringVector")
      .def_readwrite("string_vector", &zengine::StringVector::string_vector, luabind::return_stl_iterator)
      .def("size", &zengine::StringVector::size),

    luabind::class_<ConfigFile>("ConfigFile")
      .scope [
        luabind::def("GetInstance", &ConfigFile::GetInstance)
      ]
      .def("GetString", &ConfigFile::GetString)
      .def("GetStringList", &ConfigFile::GetStringList)
      .def("GetCStringList", &ConfigFile::GetCStringList)
      .def("GetSectionKey", &ConfigFile::GetSectionKey)
      .def("GetSectionName", &ConfigFile::GetSectionName),

    luabind::class_<ZEngineContext>("ZEngineContext")
      .def("PostTaskByInstanceName", &ZEngineContext::PostTaskByInstanceName)
      .def("PostTaskByInstanceID", &ZEngineContext::PostTaskByInstanceID)
      .def("PostTaskToIOThread", &ZEngineContext::PostTaskToIOThread)
      .def("PostTaskToMainThread", &ZEngineContext::PostTaskToMainThread)
      .def("PostTask", &ZEngineContext::PostTask)
      .def("SetTimer", &ZEngineContext::SetTimer)
      .def("KillTimer", &ZEngineContext::KillTimer)
      .def("context_type", &ZEngineContext::context_type)
      .def("instance_name", &ZEngineContext::instance_name)
      .def("instance_id", &ZEngineContext::instance_id),
   
      luabind::def("CreateZEngineThread", &CreateZEngineThread)
    //luabind::class_<ZEngineDaemon>("ZEngineDaemon")
    //  //.def("Initialize_NetEngineManager", &ZEngineDaemon::Initialize_NetEngineManager)
    //  //.def("CreateTCPConnector", &ZEngineDaemon::CreateTCPConnector)
    //  //.def("CreateTCPAcceptor", &ZEngineDaemon::CreateTCPAcceptor)
    //  .def("CreateZEngineThread", &ZEngineDaemon::CreateZEngineThread)
  ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void ZEngine_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(kZEngineModuleName, &Luabind_ZEngine_Register);
}

}
