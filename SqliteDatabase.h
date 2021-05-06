#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <io.h>

class SqliteDatabase : public IDAtabase
{
public:
	SqliteDatabase(); // the db will be constructed here
	virtual bool doesUserExist(std::string userName) override;
	virtual bool doesPasswordMatch(std::string userName, std::string password) override;
	virtual void addNewUser(std::string userName, std::string password, std::string email) override;
	void sendSqlStatement(std::string sqlStatement, int(*callback)(void*, int, char**, char**), void* data);

private:
	sqlite3* db;
};