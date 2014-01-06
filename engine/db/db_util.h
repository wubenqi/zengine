// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef DB_DB_UTIL_H_
#define DB_DB_UTIL_H_
#pragma once

#include <string>
#include <vector>
#include "base/basictypes.h"

namespace db {

enum ColumnType { 
	kColumnTypeStrEsc = 0,
	kColumnTypeStr,
	kColumnTypeInt32,
	kColumnTypeUInt32,
	kColumnTypeBlob,
	kColumnTypeStr2,
	kColumnTypeFloat,
	kColumnTypeDouble
};

struct QueryParamItem {
	int type;
	int len;
	const char *param;
};

class QueryParam : 
	public std::vector <QueryParamItem> {
public:

	void AddParamEsc(const char *param) {
		_addparam(param, static_cast<int>(strlen(param)), kColumnTypeStrEsc);
	}

	void AddParam(const char *param) {
		_addparam(param, static_cast<int>(strlen(param)), kColumnTypeStr);
	}

	void AddParam(const int *param) {
		_addparam((const char*)param, sizeof(int), kColumnTypeInt32 );
	}

	void AddParam(const float *param) {
		_addparam((const char*)param, sizeof(float), kColumnTypeFloat );
	}

	void AddParam(const uint32 *param) {
		_addparam((const char*)param, sizeof(uint32), kColumnTypeUInt32);
	}

	void AddParam(const char *param, int len) {
		_addparam((const char*)param, len, kColumnTypeStr);
	}

	void AddParamEsc(const char *param, int len) {
		_addparam(param, len, kColumnTypeBlob);
	}

	void AddParam(const double *param) {
		_addparam((const char*)param, sizeof(double), kColumnTypeDouble);
	}

	void Clear() {
		clear();
	}

private:
	void _addparam(const char *param, int len, int type) {
		QueryParamItem p;
		p.len = len;
		p.type = type;
		p.param = param;
		push_back(p);
	}
};


bool MakeQueryString(const char *query, int lmt, std::string *query_string);
bool MakeQueryString(const char *query, QueryParam *param, std::string *query_string);
bool MakeQueryString(const char *query, QueryParam *param, int lmt, std::string *query_string);


// 
class QueryAnswer {
public:
	QueryAnswer() 
		: column_count_(0)
		, row_count_(0) {
	}

	virtual ~QueryAnswer(){}

	virtual bool FetchRow() = 0;

	virtual const char *GetColumn(uint32 clm) const = 0;
	virtual uint32 GetColumnLength(uint32 clm) const = 0;
	virtual bool ColumnIsNull(uint32 clm) const = 0;

	virtual bool GetColumn(uint32 clm, uint64* val) const = 0;
	virtual bool GetColumn(uint32 clm, int64* val) const = 0;
	virtual bool GetColumn(uint32 clm, uint32* val) const = 0;
	virtual bool GetColumn(uint32 clm, int* val) const = 0;
	virtual bool GetColumn(uint32 clm, float* val) const = 0;
	virtual bool GetColumn(uint32 clm, bool* val) const = 0;

	inline uint32 GetColumnCount() const { return column_count_; }
	inline uint64 GetRowCount() const { return row_count_; }

protected:
	uint32 column_count_;
	uint64 row_count_;
};

//void StringAppendQ(std::string* dest, const char* src, int count);


enum DBSQLType {
	kDBSQLType_None = 0,
	kDBSQLType_Insert = 1,
	kDBSQLType_Update = 2,
	kDBSQLType_Delete = 3,
};

struct MyQueryParamItem {
	std::string ToNameValueString() const {
		return col_name+"="+ColValueToString();
	}

	std::string ColValueToString() const;
	const std::string& ColName() const {
		return col_name;
	}

	std::string col_name;	// 表字段列名
	QueryParamItem col;		// 查询参数
};

class MyQueryParam : 
	public std::vector<MyQueryParamItem> {
public:
	MyQueryParam()
		: sql_type_(kDBSQLType_None)
		, aid_(0) {
	}

	explicit MyQueryParam(const std::string& tb_name)
		: tb_name_(tb_name)
		, sql_type_(kDBSQLType_None)
		, aid_(0) {
	}

	void Initialize(const std::string& tb_name, DBSQLType sql_type, uint32 aid = 0) {
		tb_name_ = tb_name;
		sql_type_ = sql_type;
		aid_ = aid;
		Clear();
	}

	void ResetSQLType(DBSQLType sql_type) {
		sql_type_ = sql_type;
	}

	void ResetSQLType(uint32 aid) {
		aid_ = aid;
	}

	void AddParamEsc(const std::string& col_name, const char *param) {
		_addparam(col_name, param, static_cast<int>(strlen(param)), kColumnTypeStrEsc);
	}

	void AddParam(const std::string& col_name, const char *param) {
		_addparam(col_name, param, static_cast<int>(strlen(param)), kColumnTypeStr);
	}

	void AddParamEsc(const std::string& col_name, const std::string& param) {
		_addparam(col_name, param.c_str(), static_cast<int>(param.length()), kColumnTypeStrEsc);
	}

	void AddParam(const std::string& col_name, const std::string& param) {
		_addparam(col_name, param.c_str(), static_cast<int>(param.length()), kColumnTypeStr);
	}

	void AddParam(const std::string& col_name, const int *param) {
		_addparam(col_name, (const char*)param, sizeof(int), kColumnTypeInt32 );
	}

	void AddParam(const std::string& col_name, const float *param) {
		_addparam(col_name, (const char*)param, sizeof(float), kColumnTypeFloat );
	}

	void AddParam(const std::string& col_name, const uint32 *param) {
		_addparam(col_name, (const char*)param, sizeof(uint32), kColumnTypeUInt32);
	}

	void AddParam(const std::string& col_name, const char *param, int len) {
		_addparam(col_name, param, len, kColumnTypeStr);
	}

	void AddParamEsc(const std::string& col_name, const char *param, int len) {
		_addparam(col_name, param, len, kColumnTypeBlob);
	}

	void AddParam(const std::string& col_name, const double *param) {
		_addparam(col_name, (const char*)param, sizeof(double), kColumnTypeDouble);
	}

	void Clear() {
		clear();
	}

	inline const std::string& GetTableName() const {
		return tb_name_;
	}

	inline DBSQLType GetDBSqlType() const {
		return sql_type_;
	}

	inline uint32 GetAID() const {
		return aid_;
	}

	void ToMyQueryString(std::string* query_string) const;

private:
	void _addparam(const std::string& col_name, const char *param, int len, int type) {
		MyQueryParamItem p;
		p.col_name = col_name;
		p.col.len = len;
		p.col.type = type;
		p.col.param = param;
		push_back(p);
	}

	void ToMyInsertString(std::string* query_string) const;
	void ToMyDeleteString(std::string* query_string) const;
	void ToMyUpdateString(std::string* query_string) const;

	std::string tb_name_;
	DBSQLType sql_type_;
	uint32 aid_;
};

}

#endif
