// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef DB_MYSQL_MYSQL_DATABASE_H_
#define DB_MYSQL_MYSQL_DATABASE_H_
#pragma once


#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
//#include "base/logging.h"
#include "db/base_database.h"

struct st_mysql;
namespace db{

class MySQLDatabase :
	public virtual BaseDatabase {
public:
	MySQLDatabase();
	virtual ~MySQLDatabase();

	// 查询语句
	virtual QueryAnswer *Query(const char *q_str, size_t len, int* err);

	// 插入
	// 返回INSERT 操作产生的 ID
	virtual uint64 ExecuteInsertID(const char *q_str, size_t len, int* err);

	// 插入和更新
	virtual int Execute(const char *q_str, size_t len, int* err);

	virtual bool BeginTransaction();
	virtual bool CommitTransaction();
	virtual bool RollbackTransaction();

	virtual bool Open(const char* host, const char* user, const char* password, const char* dbname, int port);
	virtual void CloseDb();
	virtual int GetError(int err);
	virtual const std::string& DispError();

protected:
	virtual bool Ping();

private:
	int SafeQuery(const char* q_str, size_t len, int* err);

	st_mysql* my_;
};

#if 0
class MysqlDatabaseFactory :
	public DatabaseFactory {
public:
	static MysqlDatabaseFactory* GetInstance() {
		return Singleton<MysqlDatabaseFactory>::get();
	}

	virtual BaseDatabase* CreateDatabase() {
		NOTREACHED();
	}

	virtual BaseDatabase* CreateDatabase(const char* host, const char* user, const char* password, const char* dbname, int port) {
		scoped_ptr<MySQLDatabase> db(new MySQLDatabase());
		//CHECK(db.get());

		if(!db->Open(host, user, password, dbname, port)) {
			return NULL;
		}

		return db.release();
	}

private:
	friend class Singleton<MysqlDatabaseFactory>;
	friend struct DefaultSingletonTraits<MysqlDatabaseFactory>;

	MysqlDatabaseFactory() {}
	virtual ~MysqlDatabaseFactory() {}
};
#endif

}

#endif

