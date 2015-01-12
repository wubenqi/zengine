// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zengine/zengine_root.h"

#include <algorithm>
#include "base/file_util.h"
#include "base/logging.h"
#include "base2/exception.h"

//#include "base2/dyn_lib_manager.h"

#include "scriptengine/luabind_register_manager.h"
#include "zengine/script/script_file_manager.h"

#ifdef ZENGINE_STATIC_LIB
//#include "mod_db/module_db_plugin.h"
//#include "mod_net/module_net_plugin.h"
//#include "m"
//#include "zengine/zengine_plugin_factory_manager.h"

#include "zengine/zengine_plugin_dll_manager.h"
#endif

#include "zengine/script/script_file_manager.h"
#include "zengine/script/script_manager.h"
#include "zengine/zengine_thread.h"
#include "zengine/zengine_context.h"
#include "zengine/zengine_context_manager.h"

namespace zengine{

Root::Root() 
  : is_initialized_(false)
  , context_(NULL) {

  // base::ConfigFile::GetInstance();
  LuabindRegisterManager::GetInstance();
  ScriptFileManager::GetInstance();
  ZEngineContextManager::GetInstance();

#ifdef ZENGINE_STATIC_LIB
  //ModuleDBPluginFactory::GetInstance();
  //ModuleNetPluginFactory::GetInstance();
  PluginDllManager::GetInstance();
  // PluginFactoryManager::GetInstance();
#endif

  ZEngineContextManager::GetInstance();

  message_loop_ = base::MessageLoop::current();
}

Root::~Root() {
  ShutdownModules();
  UnloadModules();
}

void Root::Initialize(const base::ConfigFile* config) {
  config_file_ = config;
#ifdef ZENGINE_STATIC_LIB
  PluginDllManager::GetInstance()->Initialize();
#endif

  // ¼ÓÔØ²å¼þÄ£¿é
  base::StringVector values = config->GetStringList("Modules", "file_path");
  LoadModules(values.string_vector);

  context_ = ZEngineContextManager::GetInstance()->CreateContext(ZEngineContext::kZEngineContextType_Main, base::PlatformThread::CurrentId(), ZEngineContext::kMainInstanceName, base::MessageLoop::current());
  
  is_initialized_ = true;
}

void Root::Shutdown() {
  if (message_loop_) {
    message_loop_->PostTask(FROM_HERE, base::Bind(&Root::DoShutdown, base::Unretained(this)));
  }
}

bool Root::CreateZEngineThread(const std::string& instance_name) {
  if (instance_name==ZEngineContext::kIOInstanceName || instance_name==ZEngineContext::kMainInstanceName) {
    LOG(ERROR) << "CreateZEngineThread error, instance_name is " << instance_name << ", not equsal main or io.";
    return false;
  }

  if (zengine_threads_.find(instance_name) != zengine_threads_.end()) {
    LOG(ERROR) << "CreateZEngineThread error, instance_name " << instance_name << " existed.";
    return false;
  }

  shared_ptr<ZEngineThread> zthread(new ZEngineThread(instance_name));
  zthread->Start();
  zengine_threads_.insert(std::make_pair(instance_name, zthread));

  return true;
}

void Root::Run() {
  OnCreate();
  base::MessageLoop::current()->Run();
  OnDestroy();
}

void Root::OnCreate() {
  InitializeModules();
  context_->script_manager()->ExeScript_MainInitialize();
  context_->script_manager()->ExeScript_OnThreadInitialize(context_);
}

void Root::OnDestroy() {
  if (is_initialized_) {

    ZEngineThreadMap::iterator it;
    for (it=zengine_threads_.begin(); it!=zengine_threads_.end(); ++it) {
      it->second->Stop();
    }
    zengine_threads_.clear();

    if (context_) {
      context_->script_manager()->ExeScript_OnThreadDestroy(context_);
      context_->script_manager()->ExeScript_MainDestroy();

      context_->Destroy();
      delete context_;
      context_ = NULL;
    }
    ShutdownModules();
    is_initialized_ = false;
  }
}

void Root::DoShutdown() {
  if (message_loop_) {
    message_loop_->Quit();
  }
}

void Root::LoadModules(const std::vector<std::string>& module_names) {
  for (size_t i=0; i<module_names.size(); ++i) {
    try {
      LoadModule(module_names[i]);
    } catch (base::Exception& e) {
      LOG(FATAL) << "Root::LoadModules - load module " << module_names[i] << " exception, " << e.what();
      return;
    }
  }
}

void Root::InitializeModules() {
  for (size_t i=0; i<modules_.size(); ++i) {
    modules_[i]->Initialize();
  }
}

void Root::ShutdownModules() {
  for (size_t i=0; i<modules_.size(); ++i) {
    modules_[i]->Shutdown();
  }
}

void Root::UnloadModules() {
#ifndef ZENGINE_STATIC_LIB
  for (ModuleInstanceList::reverse_iterator it = modules_.rbegin(); it!=modules_.rend(); ++it) {
    // Call plugin shutdown
    Func_DllStopModule dll_stop_module_func = (Func_DllStopModule)((*it)->getSymbol("DllStopModule"));
    // this will call uninstallPlugin
    dll_stop_module_func();
    // Unload library & destroy
    base::DynLibManager::GetInstance()->unload(*it);
  }
  module_libs_.clear();
#else
  for (ModuleInstanceList::reverse_iterator it=modules_.rbegin(); it!=modules_.rend(); ++it) {
    PluginDllManager::GetInstance()->DllStopModule((*it)->GetPluginName());
  }
  modules_.clear();
#endif

  // now deal with any remaining plugins that were registered through other means
  for (ModuleInstanceList::reverse_iterator it=modules_.rbegin(); it!=modules_.rend(); ++it) {
    // Note this does NOT call uninstallPlugin - this shutdown is for the 
    // detail objects
    (*it)->Uninstall();
  }
  modules_.clear();
}


void Root::LoadModule(const std::string& module_name) {
#ifndef ZENGINE_STATIC_LIB
  // Load plugin library
  base::DynLib* lib = base::DynLibManager::GetInstance()->load(module_name);
  // Store for later unload
  // Check for existence, because if called 2+ times DynLibManager returns existing entry
  if (std::find(module_libs_.begin(), module_libs_.end(), lib) == module_libs_.end()) {
    module_libs_.push_back(lib);

    // Call startup function
    Func_DllStartModule dll_start_module_func = (Func_DllStartModule)lib->getSymbol("DllStartModule");
    CHECK(dll_start_module_func) << "Root::LoadModule - Cannot find symbol DllStartModule in library " << module_name;

    // This must call installPlugin
    dll_start_module_func();
  }
#else
  PluginDllManager::GetInstance()->DllStartModule(module_name);
#endif
}

void Root::UnloadModule(const std::string& module_name) {
#ifndef ZENGINE_STATIC_LIB
  for (ModuleLibList::iterator it=module_libs_.begin(); it!=module_libs_.end(); ++it) {
    if ((*it)->getName() == module_name) {
      // Call plugin shutdown
      Func_DllStopModule dll_stop_module_func = (Func_DllStopModule)((*it)->getSymbol("DllStopModule"));
      // this will call uninstallPlugin
      dll_stop_module_func();
      // Unload library & destroy
      base::DynLibManager::GetInstance()->unload(*it);
      //module_libs_.erase(it);
    }
  }
#else
  for (size_t i=0; i<modules_.size(); ++i) {
    if (modules_[i]->GetPluginName() == module_name) {
      PluginDllManager::GetInstance()->DllStopModule(module_name);
    }
  }
#endif
}

void Root::InstallModule(Plugin* module) {
  LOG(INFO) << "void Root::InstallModule - Installing plugin: " << module->GetPluginName();
  modules_.push_back(module);
  module->Install(config_file_);
  if (is_initialized_) {
    module->Initialize();
  }
  LOG(INFO) << "void Root::InstallModule - Plugin successfully installed";
 }

void Root::UninstallModule(Plugin* module) {
  std::string module_name = module->GetPluginName();
  LOG(INFO) << "void Root::UninstallModule - Uninstalling plugin: " << module_name;
  ModuleInstanceList::iterator it = std::find(modules_.begin(), modules_.end(), module);
  if (it != modules_.end()) {
    if (is_initialized_) {
      module->Shutdown();
    }
    module->Uninstall();
    //modules_.erase(it);
  }
  LOG(INFO) << "void Root::UninstallModule - Successfully uninstalled plugin: " << module_name;
}

}
