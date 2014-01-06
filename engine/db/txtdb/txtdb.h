// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//
//	游戏引擎, TXTDB数据库系统, 操作从EXCEL导出的以制表符分割的数据库
//  操作接口类似于数据库操作接口
//

#ifndef DB_TXTDB_TXTDB_H_
#define DB_TXTDB_TXTDB_H_
#pragma once

#include "base/memory/singleton.h"

#include "db/base_database.h"

namespace db {

class TXTDB :
	public virtual BaseDatabase {
public:
	TXTDB(void) {}
	virtual ~TXTDB(void) {}

/*
	virtual bool Initialize(const char* dbName);
	virtual void Destroy();

	virtual MiniDBQueryResult* QueryAll();
	void FreeQueryResult(MiniDBQueryResult* query);

	inline uint32 GetMaxIndex() {
		assert(inited_!=false);
		return max_index_+1;
	}

	//提供此函数是为了能和Storage保持兼容
	inline std::string GetFormatString() {
		assert(inited_!=false);
		return fieldInfo_.toFormatString();
	}

	inline MiniDBTableFieldInfo* GetFieldInfo() {
		return &fieldInfo_;
	}
*/

	// 查询语句
	//  从一个TXT文件里读出全部数据
	//  q_str为文件路径
	//  len问文件路径长度
	virtual QueryAnswer *Query(const char *q_str);
	virtual QueryAnswer *Query(const char *q_str, size_t len);
	virtual QueryAnswer *Query(const char *q_str, size_t len, int* err);

	// 不能调用
	virtual uint64 ExecuteInsertID(const char *q_str, size_t len, int* err);

	// 不能调用
	virtual int Execute(const char *q_str, size_t len, int* err);

	// 不能调用
	virtual bool Open(const char* host, const char* user, const char* password, const char* dbname, int port);
	virtual int GetError(int err);
	const std::string& DispError();

protected:
	QueryAnswer *_Query(const char *q_str, int* err);

/*
	MiniDB_AllResult all_result_;
	uint32 m_fieldCount;
	uint32 m_rowCount;

	//格式化字符串
	uint32 max_index_;
	MiniDBTableFieldInfo fieldInfo_;

	bool inited_;
*/
};

#if 0
class TXTDBFactory :
	public DatabaseFactory {
public:
	static TXTDBFactory* GetInstance() {
		return Singleton<TXTDBFactory>::get();
	}

	virtual BaseDatabase* CreateDatabase(const char* host, const char* user, const char* password, const char* dbname, int port) {
		return new TXTDB();
	}

private:
	friend class Singleton<TXTDBFactory>;
	friend struct DefaultSingletonTraits<TXTDBFactory>;

	TXTDBFactory() {}
	virtual ~TXTDBFactory() {}
};
#endif

}

#endif //
