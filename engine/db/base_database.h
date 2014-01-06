// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef DB_BASE_DATABASE_H_
#define DB_BASE_DATABASE_H_
#pragma once

#include "db/db_util.h"

namespace db {

// 
class BaseDatabase {
public:
	BaseDatabase() {
	}

	enum DatabaseType {
		kDatabaseTypeMysql = 0,
		kDatabaseTypeTXTDB = 1,
		kDatabaseTypeMiniDB = 2
	};

	virtual ~BaseDatabase() {
		CloseDb();
	}

	// 新建一个数据库
	static db::BaseDatabase *NewDataBase(DatabaseType db_type);

	// 查询语句
	virtual QueryAnswer *Query(const char *q_str);
	virtual QueryAnswer *Query(const char *q_str, size_t len);
	virtual QueryAnswer *Query(const char *q_str, size_t len, int* err) = 0;

	// 插入
	// 返回INSERT 操作产生的 ID
	virtual uint64 ExecuteInsertID(const char *q_str);
	virtual uint64 ExecuteInsertID(const char *q_str, size_t len);
	virtual uint64 ExecuteInsertID(const char *q_str, size_t len, int* err) = 0;

	// 插入，更新和删除
	virtual int Execute(const char *q_str);
	virtual int Execute(const char *q_str, size_t len);
	virtual int Execute(const char *q_str, size_t len, int* err) = 0;

	virtual bool BeginTransaction() { return true; }
	virtual bool CommitTransaction() { return true; }
	virtual bool RollbackTransaction() { return true; }

	virtual bool Open(const char* host, const char* user, const char* password, const char* dbname, int port) = 0;
	virtual void CloseDb(){}
	virtual int GetError(int err) = 0;
	virtual const std::string& DispError() = 0;

protected:
	virtual bool Ping() { return true; }

	std::string str_error_;
};

#if 0
class DatabaseFactory {
public:
	virtual BaseDatabase* CreateDatabase() = 0;
	virtual BaseDatabase* CreateDatabase(const char* host, const char* user, const char* password, const char* dbname, int port) = 0;

protected:
	DatabaseFactory() {}
	virtual ~DatabaseFactory ()  {}

	DISALLOW_COPY_AND_ASSIGN(DatabaseFactory);
};
#endif

}

//typedef db::BaseDatabase *BaseDatabasePtr;
//// In each DB driver, you need to provide this function:
//extern "C" {
//BaseDatabasePtr NewDataBase(void); /* return new CSomeSQLDatabase; */
//}


#endif /* _SQLDB_H_ */
