// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_db/register_helper/db_register_helper.h"

#include "luabind/luabind.hpp"
#include "luabind/out_value_policy.hpp"
#include "luabind/raw_policy.hpp"

#include "db/database.h"
#include "mod_db/module_db_dll.h"

db::CdbConnPoolMgr* GetDBConnPoolMgrByInstanceName(const std::string& instance_name);
db::CdbConnPoolMgr* GetDBConnPoolMgr();

namespace {

const char* GetColumn(db::QueryAnswer* answ, uint32 clm) {
  return answ->GetColumn(clm);
}

void ColumnToString(db::QueryAnswer* answ, uint32 clm, lua_State* L) {
  lua_pushlstring(L, answ->GetColumn(clm), answ->GetColumnLength(clm));
}

int GetColumnInt(db::QueryAnswer* answ, uint32 clm) {
  int result = 0;
  answ->GetColumn(clm, &result);
  return result;
}

float GetColumnFloat(db::QueryAnswer* answ, uint32 clm) {
  float result = 0;
  answ->GetColumn(clm, &result);
  return result;
}

void Luabind_DB_Register(lua_State* L) {
  luabind::module(L) [
    luabind::class_<db::QueryAnswer, boost::shared_ptr<db::QueryAnswer> >("QueryAnswer")
      .def("FetchRow", &db::QueryAnswer::FetchRow)
      .def("GetColumn", &GetColumn)
      .def("ColumnToString", &ColumnToString)
      .def("GetColumnLength", &db::QueryAnswer::GetColumnLength)
      .def("ColumnIsNull", &db::QueryAnswer::ColumnIsNull)
      .def("GetColumnInt", &GetColumnInt)
      .def("GetColumnFloat", &GetColumnFloat)
      .def("GetColumnCount", &db::QueryAnswer::GetColumnCount)
      .def("GetRowCount", &db::QueryAnswer::GetRowCount),

    luabind::class_<db::DatabaseConnection>("DatabaseConnection")
      .def("Query", &db::DatabaseConnection::Query)
      .def("ExecuteInsertID", &db::DatabaseConnection::ExecuteInsertID)
      .def("Execute", &db::DatabaseConnection::Execute)
      .def("BeginTransaction", &db::DatabaseConnection::BeginTransaction)
      .def("CommitTransaction", &db::DatabaseConnection::CommitTransaction)
      .def("RollbackTransaction", &db::DatabaseConnection::RollbackTransaction),

    luabind::class_<db::CdbConnPoolMgr>("CdbConnPoolMgr")
      .def(luabind::constructor<>())
      .def("Initialize", &db::CdbConnPoolMgr::Initialize)
      .def("Shutdown", &db::CdbConnPoolMgr::Shutdown)
      //.def("AsyncQuery2", &db::CdbConnPoolMgr::AsyncQuery2)
      //.def("AsyncExecute", &db::CdbConnPoolMgr::AsyncExecute)
      .def("GetFreeConnection", &db::CdbConnPoolMgr::GetFreeConnection)
      .def("SetFreeConnection", &db::CdbConnPoolMgr::SetFreeConnection),

      luabind::def("GetDBConnPoolMgrByInstanceName", &GetDBConnPoolMgrByInstanceName),
      luabind::def("GetDBConnPoolMgr", &GetDBConnPoolMgr)
  ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void DB_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_DB(), &Luabind_DB_Register);
}

}

