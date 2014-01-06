// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef DB_BASE_DB_OBJ_H_
#define DB_BASE_DB_OBJ_H_

#include <string>
#include "base/basictypes.h"

namespace db{

class Field;
class Database;

}

class BaseDBObj {
public:
	enum DBOP_TYPE {
		DB_NONE,
		DB_LOAD,
		DB_SAVE,
		DB_ADDNEW,
		DB_DELETE,
	};

	enum	ODBC_ERROR {
		DB_SAME_PRI_KEY = 2601, //重复关键字段
	};

	BaseDBObj(void) { 
		op_type_ = DB_NONE; 
		field_count = 0;
		row_count = 0;
		db_ = NULL;
	}

	virtual ~BaseDBObj(void) {}

	int32 GetErrorCode();
	std::string	 GetErrorMessage();
	inline uint32 GetFieldCount() const { return field_count; }
	inline uint32 GetRowCount() const { return row_count; }

	// 子类需要重载才能实现功能
	virtual bool Load(void* data);
	virtual bool AddNew(void* data);
	virtual bool Delete(void* data);
	virtual bool Save(void* data);

protected:
	virtual void	SerializeToObject(void* data2, db::Field* fld) {}

protected:
	DBOP_TYPE		op_type_;
	uint32 field_count;
	uint32 row_count;
	db::Database*	db_;

};

#endif
