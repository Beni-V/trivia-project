#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <io.h>
#include <string>
#include <list>
#include <vector>

class SqliteDatabase : public IDAtabase
{
public:
	SqliteDatabase(); // the db will be constructed here
	bool doesUserExist(std::string userName) override;
	bool doesPasswordMatch(std::string userName, std::string password) override;
	void addNewUser(std::string userName, std::string password, std::string email) override;
	void sendSqlStatement(std::string sqlStatement, int(*callback)(void*, int, char**, char**), void* data);
	std::vector<std::vector<std::string>> getQuestions();
	float getPlayerAverageAnswerTime(std::string userName);
	int getNumOfCorrectAnswers(std::string userName);
private:
	sqlite3* db;
};