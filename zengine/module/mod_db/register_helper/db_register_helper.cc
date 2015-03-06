// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "mod_db/register_helper/db_register_helper.h"

#include "base/basictypes.h"
#include "scriptengine/luatinker/lua_tinker.h"

#include "zengine/zengine_context.h"

#include "db/conn_pool_manager.h"
#include "mod_db/module_db_dll.h"
#include "mod_db/module_db_plugin.h"

CdbConnPoolManagerProxy* GetDBConnPoolMgrByInstanceName(const std::string& instance_name);
CdbConnPoolManagerProxy* GetDBConnPoolMgr();


namespace {

// const char* GetColumn(db::QueryAnswer* answ, uint32 clm) {
//   return answ->GetColumn(clm);
// }
// 
// void ColumnToString(db::QueryAnswer* answ, uint32 clm, lua_State* L) {
//   lua_pushlstring(L, answ->GetColumn(clm), answ->GetColumnLength(clm));
// }
// 
// int GetColumnInt(db::QueryAnswer* answ, uint32 clm) {
//   int result = 0;
//   answ->GetColumn(clm, &result);
//   return result;
// }
// 
// float GetColumnFloat(db::QueryAnswer* answ, uint32 clm) {
//   float result = 0;
//   answ->GetColumn(clm, &result);
//   return result;
// }



void Luabind_DB_Register(lua_State* L) {
  lua_tinker::class_add<db::QueryAnswer>(L, "QueryAnswer");
  lua_tinker::class_def<db::QueryAnswer>(L, "FetchRow", &db::QueryAnswer::FetchRow);
  lua_tinker::class_def<db::QueryAnswer>(L, "PrintDebugFieldNames", &db::QueryAnswer::PrintDebugFieldNames);
  lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnName", &db::QueryAnswer::GetColumnName);
  lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnLength", (uint32(db::QueryAnswer::*)(uint32) const)&db::QueryAnswer::GetColumnLength);
  // lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnLength", (uint32(db::QueryAnswer::*)(const base::StringPiece&) const)&db::QueryAnswer::GetColumnLength);
  lua_tinker::class_def<db::QueryAnswer>(L, "ColumnIsNull", (bool(db::QueryAnswer::*)(uint32) const)&db::QueryAnswer::ColumnIsNull);
  // lua_tinker::class_def<db::QueryAnswer>(L, "ColumnIsNull", (bool(db::QueryAnswer::*)(const base::StringPiece&) const)&db::QueryAnswer::ColumnIsNull);
  lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnValue", (std::string(db::QueryAnswer::*)(uint32) const)&db::QueryAnswer::GetColumnValue);
  // lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnValue", (std::string(db::QueryAnswer::*)(const base::StringPiece&) const)&db::QueryAnswer::GetColumnValue);
  lua_tinker::class_def<db::QueryAnswer>(L, "GetColumnCount", &db::QueryAnswer::GetColumnCount);
  lua_tinker::class_def<db::QueryAnswer>(L, "GetRowCount", &db::QueryAnswer::GetRowCount);

  lua_tinker::class_add<CdbConnPoolManagerProxy>(L, "CdbConnPoolManagerProxy");
  lua_tinker::class_def<CdbConnPoolManagerProxy>(L, "Query", &CdbConnPoolManagerProxy::Query);
  lua_tinker::class_def<CdbConnPoolManagerProxy>(L, "ExecuteInsertID", &CdbConnPoolManagerProxy::ExecuteInsertID);
  lua_tinker::class_def<CdbConnPoolManagerProxy>(L, "Execute", &CdbConnPoolManagerProxy::Execute);

  lua_tinker::def(L, "GetDBConnPoolMgrByInstanceName", &GetDBConnPoolMgrByInstanceName);
  lua_tinker::def(L, "GetDBConnPoolMgr", &GetDBConnPoolMgr);

//   luabind::module(L) [
//     luabind::class_<db::QueryAnswer, boost::shared_ptr<db::QueryAnswer> >("QueryAnswer")
//       .def("FetchRow", &db::QueryAnswer::FetchRow)
//       .def("GetColumn", &GetColumn)
//       .def("ColumnToString", &ColumnToString)
//       .def("GetColumnLength", &db::QueryAnswer::GetColumnLength)
//       .def("ColumnIsNull", &db::QueryAnswer::ColumnIsNull)
//       .def("GetColumnInt", &GetColumnInt)
//       .def("GetColumnFloat", &GetColumnFloat)
//       .def("GetColumnCount", &db::QueryAnswer::GetColumnCount)
//       .def("GetRowCount", &db::QueryAnswer::GetRowCount),
// 
//     luabind::class_<db::DatabaseConnection>("DatabaseConnection")
//       .def("Query", &db::DatabaseConnection::Query)
//       .def("ExecuteInsertID", &db::DatabaseConnection::ExecuteInsertID)
//       .def("Execute", &db::DatabaseConnection::Execute)
//       .def("BeginTransaction", &db::DatabaseConnection::BeginTransaction)
//       .def("CommitTransaction", &db::DatabaseConnection::CommitTransaction)
//       .def("RollbackTransaction", &db::DatabaseConnection::RollbackTransaction),
// 
//     luabind::class_<db::CdbConnPoolMgr>("CdbConnPoolMgr")
//       .def(luabind::constructor<>())
//       .def("Initialize", &db::CdbConnPoolMgr::Initialize)
//       .def("Shutdown", &db::CdbConnPoolMgr::Shutdown)
//       //.def("AsyncQuery2", &db::CdbConnPoolMgr::AsyncQuery2)
//       //.def("AsyncExecute", &db::CdbConnPoolMgr::AsyncExecute)
//       .def("GetFreeConnection", &db::CdbConnPoolMgr::GetFreeConnection)
//       .def("SetFreeConnection", &db::CdbConnPoolMgr::SetFreeConnection),
// 
//       luabind::def("GetDBConnPoolMgrByInstanceName", &GetDBConnPoolMgrByInstanceName),
//       luabind::def("GetDBConnPoolMgr", &GetDBConnPoolMgr)
//   ];
}

}

//////////////////////////////////////////////////////////////////////////

#include "scriptengine/luabind_register_manager.h"

namespace zengine {

void DB_Register_Helper() {
  LuabindRegisterManager::GetInstance()->RegisterLuabindRegisterFunc(ModuleName_DB(), &Luabind_DB_Register);
}

}

