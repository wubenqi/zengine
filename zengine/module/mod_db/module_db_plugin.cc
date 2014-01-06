// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_db/module_db_plugin.h"

#include "base/stl_util.h"
#include "db/database.h"

#include "zcommon/config_util.h"
#include "zengine/config_file.h"
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

void ModuleDBPlugin::Install() {
  DB_Register_Helper();
}

void ModuleDBPlugin::Initialize() {
  ConfigFile* config = ConfigFile::GetInstance();
  
  zengine::StringVector instances = config->GetStringList(kDBModuleName.c_str(), "instance_name");
  for (size_t i=0; i<instances.size(); ++i) {
    if (config->CheckBySection(instances.string_vector[i].c_str())) {
      DatabaseDescrInfo dbdi;
      const char* val = NULL;
      dbdi.instance_name = instances.string_vector[i];
      val = config->GetString(instances.string_vector[i].c_str(), "host");
      if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":host is empty."; break; }
      dbdi.host_name = val;
      val = config->GetString(instances.string_vector[i].c_str(), "user");
      if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":user is empty."; break; }
      dbdi.user_name = val;
      val = config->GetString(instances.string_vector[i].c_str(), "passwd");
      if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":passwd is empty."; break; }
      dbdi.password = val;
      val = config->GetString(instances.string_vector[i].c_str(), "name");
      if (!val) { LOG(ERROR) << "ModuleDBPlugin::Initialize - DatabaseDescrInfo:" << instances.string_vector[i] << ":name is empty."; break; }
      dbdi.db_name = val;

      dbdi.port = config->GetInt(instances.string_vector[i].c_str(), "port", 3306);
      dbdi.type = config->GetInt(instances.string_vector[i].c_str(), "type", 1);
      dbdi.max_conn_size = config->GetInt(instances.string_vector[i].c_str(), "conn_count", 3);

      scoped_ptr<db::CdbConnPoolMgr> db_conn_pool_mgr(new db::CdbConnPoolMgr());
      bool inited =  db_conn_pool_mgr->Initialize(dbdi.host_name.c_str(), dbdi.user_name.c_str(), dbdi.password.c_str(), dbdi.db_name.c_str(), dbdi.port, dbdi.max_conn_size);
      if (inited) {
        db_conn_pool_mgrs_.push_back(db_conn_pool_mgr.release());
        db_instance_names_.push_back(instances.string_vector[i]);
      }
    }
  }
}

void ModuleDBPlugin::Shutdown() {
  db_instance_names_.clear();
  STLDeleteElements(&db_conn_pool_mgrs_);
}

void ModuleDBPlugin::Uninstall() {
}

db::CdbConnPoolMgr* ModuleDBPlugin::GetDBConnPoolMgrByInstanceName(const std::string& instance_name) {
  db::CdbConnPoolMgr* connDB = NULL;
  for (size_t i=0; i<db_instance_names_.size(); ++i) {
    if (instance_name == db_instance_names_[i]) {
      connDB = db_conn_pool_mgrs_[i];
      break;
    }
  }
  return connDB;
}

db::CdbConnPoolMgr* ModuleDBPlugin::GetDBConnPoolMgr() {
  return db_conn_pool_mgrs_.empty()?NULL:db_conn_pool_mgrs_[0];
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
