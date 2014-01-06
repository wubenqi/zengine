// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_DB_MODULE_NET_PLUGIN_H_
#define MOD_DB_MODULE_NET_PLUGIN_H_
#pragma once

#include <vector>
#include "base/memory/singleton.h"
#include "zengine/zengine_plugin.h"
//#include "db/db_conn_pool_manager.h"

namespace db {
class CdbConnPoolMgr;
}

namespace zengine {

class ModuleDBPlugin : public Plugin {
public:
  ModuleDBPlugin() {}
  virtual ~ModuleDBPlugin() {}

  static const std::string& GetModuleDBName();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install();
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();

  bool IsInitialized() {
    return !db_conn_pool_mgrs_.empty();
  }

  db::CdbConnPoolMgr* GetDBConnPoolMgrByInstanceName(const std::string& instance_name);
  db::CdbConnPoolMgr* GetDBConnPoolMgr();


private:
  std::vector<db::CdbConnPoolMgr*> db_conn_pool_mgrs_;
  std::vector<std::string> db_instance_names_;
};

class ModuleDBPluginFactory : public PluginFactory {
public:
  static ModuleDBPluginFactory* GetInstance() {
    return Singleton<ModuleDBPluginFactory>::get();
  }

  virtual const std::string& GetPluginName() const;
  virtual Plugin* CreateInstance();
  virtual void DestroyInstance(Plugin* module);

private:
  friend struct DefaultSingletonTraits<ModuleDBPluginFactory>;

  ModuleDBPluginFactory() {}
  virtual ~ModuleDBPluginFactory ()  {}
};

}

#endif
