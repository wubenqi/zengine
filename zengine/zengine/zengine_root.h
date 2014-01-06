// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZENGINE_ZENGINE_ROOT_H_
#define ZENGINE_ZENGINE_ROOT_H_
#pragma once

#include <vector>

#include "base/memory/singleton.h"
#include "base/file_path.h"

#include "base2/shared_ptr.h"

#ifndef ZENGINE_STATIC_LIB
#include "base2/dyn_lib.h"
#include "base2/dyn_lib_manager.h"
#endif

#include "zengine/zengine_plugin.h"

class ConfigFile;
class ZEngineThread;
class ZEngineContext;
class MessageLoop;
namespace zengine {

class Root {
public:
#ifndef ZENGINE_STATIC_LIB
  typedef std::vector<base::DynLib*> ModuleLibList;
  //typedef void (*Func_DllStartModule)(void);
  //typedef void (*Func_DllStopModule)(void);
#endif
  typedef std::vector<Plugin*> ModuleInstanceList;


  static Root* GetInstance() {
    return Singleton<Root>::get();
  }

  void Initialize(const ConfigFile* config);
  void Shutdown();

  /** Returns whether the system is initialised or not. */
  bool IsInitialized(void) const { return is_initialized_; }

  void Run();

  void OnCreate();
  void OnDestroy();

  bool CreateZEngineThread(const std::string& instance_name);

  void LoadModules(const std::vector<std::string>& module_names);
  void InitializeModules();
  void ShutdownModules();
  void UnloadModules();
  void LoadModule(const std::string& module_name);
	void UnloadModule(const std::string& module_name);
	void InstallModule(Plugin* module);
	void UninstallModule(Plugin* module);

	const ModuleInstanceList& GetInstalledModules() const { return modules_; }


private:
  friend struct DefaultSingletonTraits<Root>;
  void DoShutdown();

  Root();
  virtual ~Root();

#ifndef ZENGINE_STATIC_LIB
  ModuleLibList module_libs_;
#endif
  ModuleInstanceList modules_;
  bool is_initialized_;

  typedef base::hash_map<std::string, base::SharedPtr<ZEngineThread> > ZEngineThreadMap;
  ZEngineThreadMap zengine_threads_;
  ZEngineContext* context_;

  MessageLoop* message_loop_;
  //ZEngineContextManager* context_manager_;

  DISALLOW_COPY_AND_ASSIGN(Root);
};


}

#endif
