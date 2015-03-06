// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_db/module_db_plugin.h"

#include "base/stl_util.h"
#include "db/base_database.h"
#include "db/conn_pool_manager.h"

#include "base2/config_file.h"

#include "mod_db/register_helper/db_register_helper.h"

namespace zengine {

const std::string kDBModuleName("mod_db");

const std::string& ModuleDBPlugin::GetModuleDBName() {
  return kDBModuleName;
}
//////////////////////////////////////////////////////////////////////////
const std::string& ModuleDBPlugin::GetPluginName() const {
  return kDBModuleName;
}

void ModuleDBPlugin::Install(const base::ConfigFile* config_file) {
  Plugin::Install(config_file);
  DB_Register_Helper();
}

void ModuleDBPlugin::Initialize() {
  const base::ConfigFile* config = config_file_;
  
  base::StringVector instances = config->GetStringList(kDBModuleName.c_str(), "instance_name");
  for (size_t i=0; i<instances.size(); ++i) {
    if (config->CheckBySection(instances.string_vector[i].c_str())) {
      base::DBAddrInfo dbdi;
      dbdi.ParseFromConfigFile(*config, instances.string_vector[i].c_str());
//       const char* val = NULL;
//       dbdi.instance_name = instances.string_vector[i];
//       val = config->GetString(instances.string_vector[i].c_str(), "host");
//       if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":host is empty."; break; }
//       dbdi.host_name = val;
//       val = config->GetString(instances.string_vector[i].c_str(), "user");
//       if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":user is empty."; break; }
//       dbdi.user_name = val;
//       val = config->GetString(instances.string_vector[i].c_str(), "passwd");
//       if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":passwd is empty."; break; }
//       dbdi.password = val;
//       val = config->GetString(instances.string_vector[i].c_str(), "name");
//       if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":name is empty."; break; }
//       dbdi.db_name = val;
// 
//       dbdi.port = config->GetInt(instances.string_vector[i].c_str(), "port", 3306);
//       dbdi.type = config->GetInt(instances.string_vector[i].c_str(), "type", 1);
//       dbdi.max_conn_size = config->GetInt(instances.string_vector[i].c_str(), "conn_count", 3);

      scoped_ptr<db::CdbConnPoolManager> db_conn_pool_mgr(new db::CdbConnPoolManager());
      bool inited =  db_conn_pool_mgr->Initialize(dbdi); //dbdidbdi.host_name.c_str(), dbdi.user_name.c_str(), dbdi.password.c_str(), dbdi.db_name.c_str(), dbdi.port, dbdi.max_conn_size);
      if (inited) {
        CdbConnPoolManagerProxy* conn_pool = new CdbConnPoolManagerProxy(db_conn_pool_mgr.release());
        db_conn_pool_mgrs_.insert(std::make_pair(instances.string_vector[i], conn_pool));
      }
    }
  }

}

void ModuleDBPlugin::Shutdown() {
  STLDeleteValues(&db_conn_pool_mgrs_);
}

void ModuleDBPlugin::Uninstall() {
}

CdbConnPoolManagerProxy* ModuleDBPlugin::GetDBConnPoolMgrByInstanceName(const std::string& instance_name) {
  CdbConnPoolManagerProxy* conn = NULL;
  linked_hash_map<std::string, CdbConnPoolManagerProxy*>::iterator it;
  for (it = db_conn_pool_mgrs_.begin(); it!=db_conn_pool_mgrs_.end(); ++it) {
    if (instance_name == it->first) {
      conn = it->second;
      break;
    }
  }
  return conn;
}

CdbConnPoolManagerProxy* ModuleDBPlugin::GetDBConnPoolMgr() {
  return db_conn_pool_mgrs_.empty()?NULL:db_conn_pool_mgrs_.begin()->second;
}

//////////////////////////////////////////////////////////////////////////
const std::string& ModuleDBPluginFactory::GetPluginName() const {
  return kDBModuleName;
}

Plugin* ModuleDBPluginFactory::CreateInstance() {
  return new ModuleDBPlugin();
}

void ModuleDBPluginFactory::DestroyInstance(Plugin* module) {
  ModuleDBPlugin* mod_db = reinterpret_cast<ModuleDBPlugin*>(module);
  if (mod_db) {
    delete mod_db;
    mod_db = NULL;
  }
}

}
