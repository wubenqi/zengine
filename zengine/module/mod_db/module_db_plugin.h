// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef MOD_DB_MODULE_NET_PLUGIN_H_
#define MOD_DB_MODULE_NET_PLUGIN_H_

#include "base/memory/singleton.h"
#include "base2/linked_hash_map.h"
#include "zengine/zengine_plugin.h"

#include "db/conn_pool_manager.h"

class CdbConnPoolManagerProxy {
public:
  CdbConnPoolManagerProxy(db::CdbConnPoolManager* conn_pool)
    : conn_pool_(conn_pool) {
  }

  ~CdbConnPoolManagerProxy() {
  }

  // 查询语句
  // 返回值如果为NULL,有两种情况:
  //  1. 无记录
  //  2. 执行查询时发生了错误
  // 如果为NULL，我们可以调用GetLastError()来检查是否是无记录还是发生了其它错误
  db::QueryAnswer* Query(const char* q_str) {
    db::ScopedPtr_DatabaseConnection conn(conn_pool_.get());
    tmp_answ_.reset(conn->Query(q_str));
    return  tmp_answ_.get();
  };

  // 插入
  // 返回INSERT 操作产生的 ID
  // 如果返回值为0,出错
  uint64 ExecuteInsertID(const char* q_str) {
    db::ScopedPtr_DatabaseConnection conn(conn_pool_.get());
    return conn->ExecuteInsertID(q_str);
  }

  // 插入，更新和删除
  // 返回值为>=0，成功，受影响的行数
  // <0 出错
  int Execute(const char* q_str) {
    db::ScopedPtr_DatabaseConnection conn(conn_pool_.get());
    return conn->Execute(q_str);
  }

private:
  scoped_ptr<db::CdbConnPoolManager> conn_pool_;
  scoped_ptr<db::QueryAnswer> tmp_answ_;
};

namespace zengine {

class ModuleDBPlugin : public Plugin {
public:
  ModuleDBPlugin() {}
  virtual ~ModuleDBPlugin() {}

  static const std::string& GetModuleDBName();

  //////////////////////////////////////////////////////////////////////////
  virtual const std::string& GetPluginName() const;
  virtual void Install(const base::ConfigFile* config_file);
  virtual void Initialize();
  virtual void Shutdown();
  virtual void Uninstall();

  bool IsInitialized() {
    return !db_conn_pool_mgrs_.empty();
  }

  CdbConnPoolManagerProxy* GetDBConnPoolMgrByInstanceName(const std::string& instance_name);
  CdbConnPoolManagerProxy* GetDBConnPoolMgr();


private:
  linked_hash_map<std::string, CdbConnPoolManagerProxy*> db_conn_pool_mgrs_;
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
