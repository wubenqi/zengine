// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "db/database.h"

#include "base/memory/scoped_ptr.h"
#include "db/mysql/mysql_database.h"

namespace db {

boost::shared_ptr<QueryAnswer> DatabaseConnection::Query(const char *q_str) {
  return boost::shared_ptr<QueryAnswer>(db->Query(q_str));
}

uint64 DatabaseConnection::ExecuteInsertID(const char *q_str) {
  return db->ExecuteInsertID(q_str);
}

int DatabaseConnection::Execute(const char *q_str) {
  return db->Execute(q_str);
}

bool DatabaseConnection::BeginTransaction() {
  return db->BeginTransaction();
}

bool DatabaseConnection::CommitTransaction() {
  return db->CommitTransaction();
}

bool DatabaseConnection::RollbackTransaction() {
  return db->RollbackTransaction();
}

CdbConnPoolMgr::CdbConnPoolMgr() {
  connection_count_ = 0;
  port_ = 0;
  connections_ = NULL;
}

CdbConnPoolMgr::~CdbConnPoolMgr() {
  if (connections_) {
    for(int32 i = 0; i < connection_count_; ++i) {
      if (connections_[i]) {
        if(connections_[i]->db) {
          delete connections_[i]->db;
        }
        delete connections_[i];
      }
    }
    delete[] connections_;
  }
}

bool CdbConnPoolMgr::Initialize(const char* host, const char* user, const char* password, const char* dbname, int port, uint32 conn_count) {

  host_name_.assign(host);
  connection_count_ = conn_count;
  user_name_.assign(user);
  password_.assign(password);
  database_name_.assign(dbname);
  port_ = port;

  connections_ = new DatabaseConnection*[connection_count_];

  int i=0;
  for (i = 0; i < connection_count_; ++i)
    connections_[i] = NULL;

  for (i=0; i<connection_count_; ++i) {
    scoped_ptr<MySQLDatabase> db(new MySQLDatabase());
    CHECK(db.get());

    if(!db->Open(host_name_.c_str(), user_name_.c_str(), password_.c_str(), database_name_.c_str(), port_)) {
      return false;
    }

    connections_[i] = new DatabaseConnection();
    connections_[i]->db = db.release();
  }

  return true;
}

void CdbConnPoolMgr::Shutdown() {

}

DatabaseConnection *CdbConnPoolMgr::GetFreeConnection() {
  uint32 i = 0;
  int32 counts = 0;
  for (;;) {
    DatabaseConnection* con = connections_[((i++) % connection_count_)];
    if(con->busy_mutex.Try())
      return con;

    if ((++counts) >= (20*connection_count_)) {
      LOG(WARNING) << "The Database connections are busy now";
      counts = 0;
    }
  }

  return NULL;

}

}

