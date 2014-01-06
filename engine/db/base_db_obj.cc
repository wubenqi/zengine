// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "db/base_db_obj.h"

bool BaseDBObj::Load(void* data) {
	op_type_ = DB_LOAD;
	return true;
}

bool BaseDBObj::AddNew(void* data) {
	op_type_ = DB_ADDNEW;
	return true;
}

bool BaseDBObj::Delete(void* data) {
	op_type_ = DB_DELETE;
	return true;
}

bool BaseDBObj::Save(void* data) {
	op_type_ = DB_DELETE;
	return true;
}

int32 BaseDBObj::GetErrorCode() {
	return 0;
}

std::string BaseDBObj::GetErrorMessage() {
	return "";
}
