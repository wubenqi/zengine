// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifdef OS_WIN
#include <winsock2.h>
#endif

#include "db/mysql/mysql_database.h"

#include "base/logging.h"
#include "base/string_util.h"
#include "base/string_number_conversions.h"

#include <mysql.h>
#include <mysqld_error.h>

namespace db {

enum DBError {
	kDBErrorUnknown = 0,
	kDBErrorDupEntry,
	kDBErrorMYSQLNotInited,
	kDBErrorQueryIsNull,
};

class MySQLAnswer
	: public QueryAnswer {
public:
	MySQLAnswer(MYSQL_RES *res, uint32 columns, uint64 rows)
		: QueryAnswer() {
			lengths_ = NULL;
			res_ = res;
			column_count_ = columns;
			row_count_ = rows;
	}
	virtual ~MySQLAnswer() {
		if (res_) {
			mysql_free_result(res_);
		}
		res_ = 0;
		lengths_ = NULL;
	}

	virtual bool FetchRow() {
		if (res_ && (row_ = mysql_fetch_row(res_))) {
			lengths_ = mysql_fetch_lengths(res_);
			return true;
		} else {
			return false;
		}
	}

	virtual const char *GetColumn(uint32 clm) const {
		return row_[clm];
	}

	virtual uint32 GetColumnLength(uint32 clm) const {
		// if (!lengths_ && res_) {
		//	lengths_ = mysql_fetch_lengths(res_);
		// }

		CHECK(lengths_);
		CHECK(res_);
		return static_cast<uint32>(lengths_[clm]);
	}

	virtual bool ColumnIsNull(uint32 clm) const {
		return row_[clm]==NULL;
	}


	virtual bool GetColumn(uint32 clm, uint64* val) const {
		if (row_[clm]) {
			int64 val2;
			bool ret = base::StringToInt64(row_[clm], &val2);
			if (ret) *val = (uint64)(val2);
			return ret;
		} else {
			*val = 0;
			return false;
		}
	}

	virtual bool GetColumn(uint32 clm, int64* val) const {
		if (row_[clm]) {
			return base::StringToInt64(row_[clm], val);
		} else {
			*val = 0;
			return false;
		}
	}

	virtual bool GetColumn(uint32 clm, uint32 *val) const {
		if (row_[clm]) {
			*val = strtoul(row_[clm], NULL, 10);
			return true;
		} else {
			*val = 0;
			return false;
		}
	}

	virtual bool GetColumn(uint32 clm, int *val) const {
		if (row_[clm]) {
			*val = atoi(row_[clm]);
			return true;
		} else {
			*val = 0;
			return false;
		}
	}

	virtual bool GetColumn(uint32 clm, float* val) const {
		if (row_[clm]) {
			double val2;
			// TODO 多此一举
			std::string s_clm(row_[clm], GetColumnLength(clm));
			bool ret = base::StringToDouble(s_clm, &val2);
			if (ret) *val = (float)(val2);
			return ret;
		} else {
			*val = 0.0;
			return false;
		}
	}

	virtual bool GetColumn(uint32 clm, bool* val) const {
		if (row_[clm]) {
			int val2;
			bool ret = base::StringToInt(row_[clm], &val2);
			if (ret) *val = (val2==0 ? false:true);
			return ret;
		} else {
			*val = false;
			return false;
		}
	}

protected:
	MYSQL_RES *res_;
	MYSQL_ROW row_;
	unsigned long *lengths_;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MySQLDatabase::MySQLDatabase()
	: my_(NULL) {
}

MySQLDatabase::~MySQLDatabase() {
	CloseDb();
}

QueryAnswer *MySQLDatabase::Query(const char *q_str, size_t len, int* err) {
	if (my_ == NULL) {
		if(err) *err = kDBErrorMYSQLNotInited;
		LOG(ERROR) << "Mysql connection not initialized";
		return NULL;
	}

	if (q_str == NULL || len==0) {
		LOG(ERROR) << "Querystring is null";
		if(err) *err = kDBErrorQueryIsNull;
		return NULL;
	}
	
	MySQLAnswer *answ = NULL;

	int er=0;
	if (SafeQuery(q_str, len, &er)) {
		if (err) {
			*err = GetError(er);
		}
		if (er != ER_DUP_ENTRY) {
			// plogger->log(CAT_SQL, L_ERR, "Error: %s in <%s>\n", mysql_error(my_), query->m_query);
		}

		LOG(ERROR) << DispError() << ". In sql: " << q_str;

	} else {
		if (err) *err = 0;

		MYSQL_RES *res = mysql_store_result(my_);
		uint64 rows = mysql_affected_rows(my_);
		uint32 fields = mysql_field_count(my_);

		if (res==NULL || rows==0 || fields==0) {
			if (res != NULL)
				mysql_free_result(res);
		} else  {
			answ = new MySQLAnswer(res, fields, rows);
			answ->FetchRow();
		}
	}
	return answ;
}

uint64 MySQLDatabase::ExecuteInsertID(const char *q_str, size_t len, int* err) {
	if (my_ == NULL) {
		LOG(ERROR) << "Mysql connection not initialized";
		if(err) *err = kDBErrorMYSQLNotInited;
		return 0;
	}

	if (q_str == NULL || len==0) {
		LOG(ERROR) << "Querystring is null";
		if(err) *err = kDBErrorQueryIsNull;
		return 0;
	}

	uint64 ret = 0;
	int er=0;
	if (SafeQuery(q_str, len, &er)) {
		if (err) {
			*err = GetError(er);
		}
		if (er != ER_DUP_ENTRY) {
			// plogger->log(CAT_SQL, L_ERR, "Error: %s <%s>\n", mysql_error(my_), query->m_query);
		}
		LOG(ERROR) << DispError() << ". In sql: " << q_str;
	} else {
		if (err) *err = 0;
		ret = mysql_insert_id(my_);
	}

	return ret;
}

int MySQLDatabase::Execute(const char *q_str, size_t len, int* err) {
	if (my_ == NULL) {
		LOG(ERROR) << "Mysql connection not initialized";
		if(err) *err = kDBErrorMYSQLNotInited;
		return  2000;
	}

	if (q_str == NULL || len==0) {
		LOG(ERROR) << "Querystring is null";
		if(err) *err = kDBErrorQueryIsNull;
		return 2000;
	}

	int er=0;
	int res = SafeQuery(q_str, len, &er);
	if (err) {
		if (res) {
			*err = GetError(er);
			LOG(ERROR) << DispError() << ". In sql: " << q_str;
		} else {
			*err = 0;
		}
	}

	return res;
}

namespace {

const char kBeginTransaction[] = "START TRANSACTION";
const char kCommitTransaction[] = "COMMIT";
const char kRollbackTransaction[] = "ROLLBACK";

const char kSetNamesUTF8[] = "SET NAMES `utf8`";
const char kSetCharactesUTF8[] = "SET CHARACTER SET `utf8`";

}

bool MySQLDatabase::BeginTransaction() {
	return 0==BaseDatabase::Execute(kBeginTransaction);
}

bool MySQLDatabase::CommitTransaction() {
	return 0==BaseDatabase::Execute(kCommitTransaction);
}

bool MySQLDatabase::RollbackTransaction() {
	int er = 0;
	return 0==BaseDatabase::Execute(kRollbackTransaction);
}

bool MySQLDatabase::Open(const char* host, const char* user, const char* password, const char* dbname, int port) {
	MYSQL *my_temp;
	my_temp = mysql_init(NULL);
	if (!my_temp) {
		LOG(ERROR) << "Could not initialize Mysql connection";
		return false;
	}

	mysql_options(my_temp, MYSQL_SET_CHARSET_NAME, "utf8");

/*
	char const* unix_socket;
#if defined(OS_WIN)
	if (host==".") {
		// named pipe use option (Windows)
		unsigned int opt = MYSQL_PROTOCOL_PIPE;
		mysql_options(my_temp, MYSQL_OPT_PROTOCOL, (char const*)&opt);
		port = 0;
		unix_socket = 0;
	} else {
		// generic case
		unix_socket = 0;
	}
#else
	if (host==".") {
		// socket use option (Unix/Linux)
		unsigned int opt = MYSQL_PROTOCOL_SOCKET;
		mysql_options(my_temp, MYSQL_OPT_PROTOCOL, (char const*)&opt);
		host = "localhost";
		port = 0;
		unix_socket = port_or_socket.c_str();
	} else {
		// generic case
		unix_socket = 0;
	}
#endif
*/

	my_ = mysql_real_connect(my_temp, host, user, password, dbname, port, NULL, 0);

	if (my_) {
		LOG(INFO) << "Connected to MySQL database at " << host;
		LOG(INFO) << "MySQL client library: " <<  mysql_get_client_info();
		LOG(INFO) << "MySQL server ver: %s " << mysql_get_server_info(my_);

		if (!mysql_autocommit(my_, 1))
			LOG(INFO) << "AUTOCOMMIT SUCCESSFULLY SET TO 1";
		else
			LOG(INFO) << "AUTOCOMMIT NOT SET TO 1";

		// set connection properties to UTF8 to properly handle locales for different
		// server configs - core sends data in UTF8, so MySQL must expect UTF8 too
		BaseDatabase::Execute(kSetNamesUTF8);
		BaseDatabase::Execute(kSetCharactesUTF8);

#if MYSQL_VERSION_ID >= 50003
		my_bool my_true = (my_bool)1;
		if (mysql_options(my_, MYSQL_OPT_RECONNECT, &my_true))
			LOG(INFO) << "Failed to turn on MYSQL_OPT_RECONNECT.";
		else
			LOG(INFO) << "Successfully turned on MYSQL_OPT_RECONNECT.";
#else
#warning "Your mySQL client lib version does not support reconnecting after a timeout.\nIf this causes you any trouble we advice you to upgrade your mySQL client libs to at least mySQL 5.0.13 to resolve this problem."
#endif
		return true;

	} else {
		LOG(ERROR) << "Could not connect to MySQL database at " 
			<< host 
			<< ": " 
			<< mysql_error(my_temp);
		mysql_close(my_temp);
		return false;
	}
}

int MySQLDatabase::GetError(int err) {
	switch(err) {
		case ER_DUP_ENTRY:
			return kDBErrorDupEntry;
		default:
			return kDBErrorUnknown;
	}
}

const std::string& MySQLDatabase::DispError(void) {
	const char *str;
	str = mysql_error(my_);
	str_error_.assign(str);
	return str_error_;
}

void MySQLDatabase::CloseDb() {
	if (my_) {
		mysql_close(my_);
		my_ = NULL;
	}
	
}

int MySQLDatabase::SafeQuery(const char* q_str, size_t len, int* err) {
	int res = mysql_real_query(my_, q_str, len);
	if (res) {
		*err = mysql_errno(my_);
		switch (*err) {
			case 2006:  // Mysql server has gone away
			case 2008:  // Client ran out of memory
			case 2013:  // Lost connection to sql server during query
			case 2055:  // Lost connection to sql server - system error
			case 1053:  // Lost connection to sql server - system error
				// usleep(5000000);
				// TODO reconnect
				// return mysql_real_query(my_, q_str, len);
				break;
		}
	}
	return res;
}

bool MySQLDatabase::Ping() {
	int er = mysql_ping(my_);
	if (er != 0) {
		// rv = _Reconnect(static_cast<MySQLDatabaseConnection*>(con_ptr));
	}
	return er==0;
}

}

//// Functions for use after dlopen()ing
//SQLDatabasePtr NewDataBase(void) {
//	return new db::MySQLDatabase;
//}
