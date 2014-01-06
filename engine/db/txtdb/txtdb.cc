// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "db/txtdb/txtdb.h"

#include <fstream>
#include "base/logging.h"

#include "base/stl_util.h"
#include "base/string_util.h"
#include "base/string_split.h"
#include "base/string_number_conversions.h"

namespace db{

/*
// TXTDBQueryResult
TXTDBQueryResult::TXTDBQueryResult(MiniDB_AllResult& all_result, uint32 fields, uint32 rows)
	: MiniDBQueryResult(fields, rows)
	, all_result_(all_result) {
	m_currentRow = new MiniDBField[fields];
	cur_pos_ = 0;
}

TXTDBQueryResult::~TXTDBQueryResult() {
	delete[] m_currentRow;
}

bool TXTDBQueryResult::NextRow() {
	//MYSQL_ROW row = mysql_fetch_row(m_result);
	if (cur_pos_==all_result_.size()) {
		return false;
	}

	for(uint32 i = 0; i < m_fieldCount; ++i)
		m_currentRow[i].SetValue(all_result_[cur_pos_][i].c_str());
	cur_pos_++;
	return true;
}

bool TXTDBQueryResult::ReUse() {
	cur_pos_ = 0;
	return NextRow();
}

// TXTDB
TXTDB::TXTDB(void) {
	m_fieldCount = 0;
	m_rowCount = 0;
	max_index_ = 0;
	inited_ = false;
}

bool TXTDB::Initialize(const char* dbName) {
	Destroy();
	std::string dbName2 = "";
	dbName2 += dbName;
	std::ifstream db_ifs(dbName2.c_str());
	if (!db_ifs) {
		return false;
	}

	if (!fieldInfo_.Initialize(db_ifs)) {
		return false;
	}

	std::string line;
	uint32 idxVal = 0;

	while(std::getline(db_ifs, line)) {
		//忽略
		if (line.empty() || line[0]=='#'){
			continue;
		}
		MiniDB_RowResult row;
		boost::split(row, line, boost::is_any_of("\t"));
		all_result_.push_back(row);

		//得到最大索引值
		idxVal = atoi(row[0].c_str());
		if (max_index_ < idxVal) {
			max_index_ = idxVal;
		}

	}

	if (!all_result_.empty()) {
		m_fieldCount = all_result_[0].size();
		m_rowCount = all_result_.size();
	} else {
		return false;
	}
	inited_ = true;
	return true;
}

MiniDBQueryResult* TXTDB::QueryAll() {
	if (!inited_) {
		return NULL;
	}

	TXTDBQueryResult* res = new TXTDBQueryResult(all_result_, m_fieldCount, m_rowCount);
	res->NextRow();
	return res;

	//读到PROTOBUF对象里
}

void TXTDB::Destroy() {
	m_fieldCount = 0;
	m_rowCount = 0;
	max_index_ = 0;
	all_result_.clear();
	fieldInfo_.Clear();
	inited_ = false;
}

void TXTDB::FreeQueryResult(MiniDBQueryResult* query) {
	if (query) {
		delete query;
	}
}
*/

typedef std::vector<std::string> TXTDB_RowResult;
typedef std::vector<TXTDB_RowResult> TXTDB_AllResult;

class TXTDBQueryAnswer : 
	public QueryAnswer {
public:
	TXTDBQueryAnswer(TXTDB_AllResult* all_result, uint32 columns) {
		all_result_.swap(*all_result);
		row_count_ = all_result_.size();
		column_count_ = columns;
		cur_pos_ = 0;
	}
	virtual ~TXTDBQueryAnswer() {}

	virtual bool FetchRow();

	virtual const char *GetColumn(uint32 clm) const;
	virtual uint32 GetColumnLength(uint32 clm) const;
	virtual bool ColumnIsNull(uint32 clm) const;

	virtual bool GetColumn(uint32 clm, uint64* val) const;
	virtual bool GetColumn(uint32 clm, int64* val) const;
	virtual bool GetColumn(uint32 clm, uint32 *val) const;
	virtual bool GetColumn(uint32 clm, int *val) const;
	virtual bool GetColumn(uint32 clm, float* val) const;
	virtual bool GetColumn(uint32 clm, bool* val) const;

protected:
	TXTDB_AllResult all_result_;

private:
	size_t cur_pos_;
};

//////////////////////////////////////////////////////////////////////////
bool TXTDBQueryAnswer::FetchRow() {
	if (cur_pos_+1==all_result_.size()) {
		return false;
	}

	cur_pos_++;
	return true;
}

const char *TXTDBQueryAnswer::GetColumn(uint32 clm) const {
	DCHECK_LT(clm, column_count_);
	return string_as_array(const_cast<std::string*>( &(all_result_[cur_pos_][clm])));
}

uint32 TXTDBQueryAnswer::GetColumnLength(uint32 clm) const {
	DCHECK_LT(clm, column_count_);
	return all_result_[cur_pos_][clm].length();
}

bool TXTDBQueryAnswer::ColumnIsNull(uint32 clm) const {
	return all_result_[cur_pos_][clm].empty();
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, uint64* val) const {
	DCHECK_LT(clm, column_count_);
	int64 val2;
	bool ret = base::StringToInt64(all_result_[cur_pos_][clm], &val2);
	if(ret) *val = (uint64)val2;
	return ret;
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, int64* val) const {
	DCHECK_LT(clm, column_count_);
	return base::StringToInt64(all_result_[cur_pos_][clm], val);
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, uint32 *val) const {
	DCHECK_LT(clm, column_count_);
	int val2;
	bool ret = base::StringToInt(all_result_[cur_pos_][clm], &val2);
	if(ret) *val = (uint32)val2;
	return ret;
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, int *val) const {
	DCHECK_LT(clm, column_count_);
	return base::StringToInt(all_result_[cur_pos_][clm], val);
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, float* val) const {
	DCHECK_LT(clm, column_count_);
	double val2;
	bool ret = base::StringToDouble(all_result_[cur_pos_][clm], &val2);
	if(ret) *val = (float)val2;
	return ret;
}

bool TXTDBQueryAnswer::GetColumn(uint32 clm, bool* val) const {
	DCHECK_LT(clm, column_count_);
	int32 val2 = 0;
	bool ret = base::StringToInt(all_result_[cur_pos_][clm], &val2);
	if(ret) *val = (val2==0 ? false:true);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
QueryAnswer *TXTDB::Query(const char *q_str) {
	int err = 0;
	return _Query(q_str, &err);
}

QueryAnswer *TXTDB::Query(const char *q_str, size_t len) {
	int err = 0;
	return _Query(q_str, &err);
}

QueryAnswer *TXTDB::Query(const char *q_str, size_t len, int* err) {
	return _Query(q_str, err);
}

QueryAnswer *TXTDB::_Query(const char *q_str, int* err) {
	if (q_str == NULL || *q_str=='\0') {
		LOG(ERROR) << "Querystring is null";
		if(err) *err = -1;
		return NULL;
	}

	std::ifstream db_ifs(q_str);
	if (!db_ifs) {
		if(err) *err = -2;
		LOG(ERROR) << "TXTDB Open table " << q_str << " error";
		return NULL;
	}

	std::string line2, line;

	size_t field_size = 0;
	std::vector<std::string> formats;
	std::vector<std::string> names;

	// 先简单一点，这里只读取前两行，取得列数
	// 读取格式
	std::getline(db_ifs, line2);
	if (!db_ifs || line2.empty() || line2[0]=='#') {
		if(err) *err = -3;
		LOG(ERROR) << "TXTDB parse table " << q_str << " format error";
		return NULL;
	}
	base::SplitString(line2, '\t', &formats);

	field_size = formats.size();

	//读取字段名
	std::getline(db_ifs, line2);
	if (!db_ifs || line2.empty() || line2[0]=='#') {
		if(err) *err = -4;
		LOG(ERROR) << "TXTDB parse table " << q_str << " field descr error";
		return NULL;
	}
	base::SplitString(line2, '\t', &names);

	if(field_size !=names.size()) {
		LOG(ERROR) << "TXTDB table " << q_str << " format error";
		if(err) *err = -5;
		return NULL;
	}

	TXTDB_AllResult all_result;// = new TXTDB_AllResult();

	while(std::getline(db_ifs, line2)) {
		//忽略
		if (line2.empty() || line2[0]=='#'){
			continue;
		}
		TXTDB_RowResult row;
		base::SplitString(line2, '\t', &row);
		if (row.size()<field_size) {
			LOG(ERROR) << "TXTDB table row data " << line2 << " format error";
			if(err) *err = -6;
			return NULL;
		}

		for (size_t i=0; i<row.size(); ++i) {
			TrimWhitespaceASCII(row[i], TRIM_ALL, &row[i]);
		}
		all_result.push_back(row);
	}
	
	if (all_result.empty()) {
		LOG(ERROR) << "TXTDB table " << q_str << " data empty";
		if(err) *err = -7;
		return NULL;
	}
	
	*err = 0;
	QueryAnswer* answ = new TXTDBQueryAnswer(&all_result, field_size);
	return answ;
}

uint64 TXTDB::ExecuteInsertID(const char *q_str, size_t len, int* err) {
	NOTREACHED() << "TXTDB's ExecuteInsertID() not reached";
	return 0;
}

int TXTDB::Execute(const char *q_str, uint32 size_t, int* err) {
	NOTREACHED() << "TXTDB's Execute() not reached";
	return 0;
}

bool TXTDB::Open(const char* host, const char* user, const char* password, const char* dbname, int port) {
	NOTREACHED() << "TXTDB's Open() reached";
	return true;
}

int TXTDB::GetError(int err) {
	return 0;
}

const std::string& TXTDB::DispError() {
	return str_error_;
}


}

