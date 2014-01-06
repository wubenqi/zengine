// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//


#ifndef DB_DB_SERIALIZABLE_H_
#define DB_DB_SERIALIZABLE_H_

#include "db/db_util.h"

namespace db{

class DBSerializable {
public:
	DBSerializable() {}
	virtual ~DBSerializable() {}

	virtual bool SerializeToDB(QueryParam* param) { return true; }
	virtual bool SerializeFromDB(const QueryAnswer& answ) { return true; }
};


}

#endif
