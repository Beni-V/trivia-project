#pragma once
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDatabase : public IDAtabase
{
public:
	SqliteDatabase(); // the db will be constructed here
	virtual bool doesUserExist(std::string userName) override;
	virtual bool doesPasswordMatch(std::string userName, std::string password) override;
	virtual void assNewUser(std::string userName, std::string password, std::string email) override;

private:
	sqlite3* db;
};