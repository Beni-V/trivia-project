#pragma once
#include "IDatabase.h"
#include "Question.h"
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
	std::vector<Question> getQuestions() override;
	float getPlayerAverageAnswerTime(std::string userName) override;
	int getNumOfCorrectAnswers(std::string userName) override;
	int getNumOfTotalAnswers(std::string userName) override;
	int getNumOfPlayerGames(std::string userName) override;
	std::vector<std::string> getUserNames() override;
private:
	sqlite3* db;
};