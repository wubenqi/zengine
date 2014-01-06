// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "db/base_database.h"

#include "mysql/mysql_database.h"
#include "txtdb/txtdb.h"

namespace db {

QueryAnswer *BaseDatabase::Query(const char *q_str) {
	int err=0;
	size_t len = strlen(q_str);
	return Query(q_str, len, &err);
}

QueryAnswer *BaseDatabase::Query(const char *q_str, size_t len) {
	int err=0;
	return Query(q_str, len, &err);
}

uint64 BaseDatabase::ExecuteInsertID(const char *q_str) {
	int err=0;
	size_t len = strlen(q_str);
	return ExecuteInsertID(q_str, len, &err);
}

uint64 BaseDatabase::ExecuteInsertID(const char *q_str, size_t len) {
	int err=0;
	return ExecuteInsertID(q_str, len, &err);
}

int BaseDatabase::Execute(const char *q_str) {
	int err=0;
	size_t len = strlen(q_str);
	return Execute(q_str, len, &err);
}

int BaseDatabase::Execute(const char *q_str, size_t len) {
	int err=0;
	return Execute(q_str, len, &err);
}

//////////////////////////////////////////////////////////////////////////

db::BaseDatabase* BaseDatabase::NewDataBase(DatabaseType db_type){
	if (db_type == kDatabaseTypeMysql) {
		return new db::MySQLDatabase();
	} else if (db_type == kDatabaseTypeMiniDB) {
		// 
	} else if (db_type == kDatabaseTypeTXTDB) {
		return new db::TXTDB();
	}
	
	return NULL;
}


}
