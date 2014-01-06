// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_protobuf/module_protobuf_plugin.h"

#include "base/logging.h"
#include "base2/dyn_lib_manager.h"

#include <google/protobuf/stubs/common.h>

#include "zengine/config_file.h"

#include "scriptengine/luabind_register_manager.h"

#include "mod_protobuf/module_protobuf_dll.h"
#include "mod_protobuf/register_helper/protobuf_register_helper.h"

struct lua_State;
#ifndef ZENGINE_STATIC_LIB
typedef void (*FN_LuabindPBModule_Register)(lua_State* L);
#else
void LuabindPB_RegisterAll(lua_State* L);
#endif

namespace zengine {

const std::string kProtobufModuleName("mod_protobuf");

const std::string& ModuleProtobufPlugin::GetModuleProtobufName() {
  return kProtobufModuleName;
}

//////////////////////////////////////////////////////////////////////////
ModuleProtobufPlugin::ModuleProtobufPlugin()
  : dyn_lib_(NULL) {
}

ModuleProtobufPlugin::~ModuleProtobufPlugin() {

}

const std::string& ModuleProtobufPlugin::GetPluginName() const {
  return kProtobufModuleName;
}

void ModuleProtobufPlugin::Install() {
  ConfigFile* config = ConfigFile::GetInstance();

  Protobuf_Register_Helper();

#ifndef ZENGINE_STATIC_LIB
  zengine::StringVector pb_dlls = config->GetStringList(kProtobufModuleName.c_str(), "file_path");
  for (size_t i=0; i<pb_dlls.size(); ++i) {
    try {
      dyn_lib_ = base::DynLibManager::GetInstance()->load(pb_dlls.string_vector[i].c_str());
      CHECK(dyn_lib_) << "Load pb_plugin: " <<  pb_dlls.string_vector[i] << " error";
      void* fn = dyn_lib_->getSymbol("LuabindPBModule_Register");
      CHECK(fn) << "getSymbol LuabindPBModule_Register error";

      FN_LuabindPBModule_Register fn_luabind_pb_module_register = (FN_LuabindPBModule_Register)fn;//dyn_lib_->getSymbol("LuabindPBModule_Register");
      CHECK(fn_luabind_pb_module_register) << "getSymbol LuabindPBModule_Register error";
      LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Protobuf(), fn_luabind_pb_module_register);

    } catch (std::exception& e) {
      LOG(ERROR) << e.what();
    } catch (...) {
      LOG(ERROR) << "Invalid error";
    }
  }
#else
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_Protobuf(), &LuabindPB_RegisterAll);
#endif
}

void ModuleProtobufPlugin::Initialize() {
}

void ModuleProtobufPlugin::Shutdown() {
}

void ModuleProtobufPlugin::Uninstall() {
  if (dyn_lib_!=NULL) {
    base::DynLibManager::GetInstance()->unload(dyn_lib_);
    dyn_lib_ = NULL;
  }
  google::protobuf::ShutdownProtobufLibrary();
}

//////////////////////////////////////////////////////////////////////////
const std::string& ModuleProtobufPluginFactory::GetPluginName() const {
  return kProtobufModuleName;
}

Plugin* ModuleProtobufPluginFactory::CreateInstance() {
  return new ModuleProtobufPlugin();
}

void ModuleProtobufPluginFactory::DestroyInstance(Plugin* module) {
  ModuleProtobufPlugin* mod = reinterpret_cast<ModuleProtobufPlugin*>(module);
  if (mod) {
    delete mod;
    mod = NULL;
  }
}

}
