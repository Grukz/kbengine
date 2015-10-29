/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2016 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KBE_DB_INTERFACE_REDIS_H
#define KBE_DB_INTERFACE_REDIS_H

#include "common.h"
#include "common/common.h"
#include "common/singleton.h"
#include "common/memorystream.h"
#include "helper/debug_helper.h"
#include "db_interface/db_interface.h"

#include "hiredis.h"
#if KBE_PLATFORM == PLATFORM_WIN32
#ifdef _DEBUG
#pragma comment (lib, "hiredis_d.lib")
#else
#pragma comment (lib, "hiredis.lib")
#endif
#endif

namespace KBEngine { 

/*
	数据库接口
	tbl_Account_Auto_increment = uint64(1)
	tbl_Account:1 = hashes(name, password, xxx)
	tbl_Account:2 = hashes(name, password, xxx)
	tbl_Account:3 = hashes(name, password, xxx(array))

	tbl_Account_xxx_values:3:size = uint64(3)
	tbl_Account_xxx_values:3:1 = val
	tbl_Account_xxx_values:3:2 = val
	tbl_Account_xxx_values:3:3 = val	
*/
class DBInterfaceRedis : public DBInterface
{
public:
	DBInterfaceRedis();
	virtual ~DBInterfaceRedis();

	static bool initInterface(DBInterface* dbi);
	
	bool ping(redisContext* pRedisContext = NULL);
	
	/**
		检查环境
	*/
	virtual bool checkEnvironment();
	
	/**
		检查错误， 对错误的内容进行纠正
		如果纠正不成功返回失败
	*/
	virtual bool checkErrors();

	/**
		与某个数据库关联
	*/
	virtual bool attach(const char* databaseName = NULL);
	virtual bool detach();

	/**
		获取数据库所有的表名
	*/
	virtual bool getTableNames( std::vector<std::string>& tableNames, const char * pattern);

	/**
		获取数据库某个表所有的字段名称
	*/
	virtual bool getTableItemNames(const char* tableName, std::vector<std::string>& itemNames);

	/**
		查询表
	*/
	virtual bool query(const char* cmd, uint32 size, bool showExecInfo = true, MemoryStream * result = NULL);
	bool query(const char* cmd, uint32 size, redisReply** pRedisReply, bool showExecInfo = true);
	bool query(bool showExecInfo, const char* format, ...);
	
	void write_query_result(redisReply* r, MemoryStream * result);
	void write_query_result_element(redisReply* r, MemoryStream * result);
		
	/**
		返回这个接口的描述
	*/
	virtual const char* c_str();

	/** 
		获取错误
	*/
	virtual const char* getstrerror();

	/** 
		获取错误编号
	*/
	virtual int getlasterror();

	/**
		创建一个entity存储表
	*/
	virtual EntityTable* createEntityTable();

	/** 
		从数据库删除entity表
	*/
	virtual bool dropEntityTableFromDB(const char* tableName);
	
	/** 
		从数据库删除entity表字段
	*/
	virtual bool dropEntityTableItemFromDB(const char* tableName, const char* tableItemName);

	/**
		锁住接口操作
	*/
	virtual bool lock();
	virtual bool unlock();

	/**
		处理异常
	*/
	virtual bool processException(std::exception & e);
	
protected:
	redisContext* pRedisContext_;
	bool hasLostConnection_;
};


}

#endif // KBE_DB_INTERFACE_REDIS_H
