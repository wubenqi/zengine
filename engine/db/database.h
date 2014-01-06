// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef DB_DATABASE_H_
#define DB_DATABASE_H_
#pragma once

#include "base/logging.h"

#include "base/synchronization/lock.h"
#include "db/base_database.h"
#include <boost/shared_ptr.hpp>

namespace db {

// 适用于SQL数据库
// 对TXTDB或MiniDB不适用
struct DatabaseConnection {
  boost::shared_ptr<QueryAnswer> Query(const char *q_str);
  uint64 ExecuteInsertID(const char *q_str);
  int Execute(const char *q_str);
  bool BeginTransaction();
  bool CommitTransaction();
  bool RollbackTransaction();

	BaseDatabase* db;
	base::Lock busy_mutex;
};

class CdbConnPoolMgr {
public:
	CdbConnPoolMgr();
	~CdbConnPoolMgr();

	bool Initialize(const char* host, const char* user, const char* password, const char* dbname, int port, uint32 conn_count);

	void Shutdown();

	DatabaseConnection *GetFreeConnection();
	inline void SetFreeConnection(DatabaseConnection* con) { if (con) con->busy_mutex.Release(); }

private:
	DatabaseConnection** connections_;

	int32 connection_count_;

	// For reconnecting a broken connection
	std::string host_name_;
	std::string user_name_;
	std::string password_;
	std::string database_name_;
	uint32 port_;
};

class ScopedPtr_DatabaseConnection {
public:
	explicit ScopedPtr_DatabaseConnection(CdbConnPoolMgr* db) 
		: db_(db) {
			db_conn_ = db_->GetFreeConnection();
	}

	~ScopedPtr_DatabaseConnection() {
		if (db_ && db_conn_) {
			db_->SetFreeConnection(db_conn_);
		}
	}

	BaseDatabase& operator*() const {
		DCHECK(db_conn_);
		DCHECK(db_conn_->db);
		return *(db_conn_->db);
	}

	BaseDatabase* operator->() const {
		DCHECK(db_conn_);
		DCHECK(db_conn_->db);
		return db_conn_->db;
	}

	BaseDatabase* get() const {
		DCHECK(db_conn_);
		return db_conn_->db;
	}

	operator bool () const {
		return db_conn_ != 0 && db_conn_->db!=0;
	}

	bool operator! () const {
		return db_conn_ == 0 || db_conn_->db==0;
	}

private:
	CdbConnPoolMgr* db_;
	DatabaseConnection* db_conn_;

	DISALLOW_COPY_AND_ASSIGN(ScopedPtr_DatabaseConnection);
};

}
#endif
