#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <io.h>

class SqliteDatabase : public IDAtabase
{
public:
	SqliteDatabase(); // the db will be constructed here
	bool doesUserExist(std::string userName) override;
	bool doesPasswordMatch(std::string userName, std::string password) override;
	void addNewUser(std::string userName, std::string password, std::string email) override;

private:
	sqlite3* db;
};