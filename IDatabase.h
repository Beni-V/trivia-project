#pragma once
#include "Question.h"
#include <string>
#include <vector>

class IDAtabase
{
public:
	virtual bool doesUserExist(std::string userName) = 0;
	virtual bool doesPasswordMatch(std::string userName, std::string password) = 0;
	virtual void addNewUser(std::string userName, std::string password, std::string email) = 0;
	virtual std::vector<Question> getQuestions() = 0;
	virtual float getPlayerAverageAnswerTime(std::string userName) = 0;
	virtual int getNumOfCorrectAnswers(std::string userName) = 0;;
	virtual int getNumOfTotalAnswers(std::string userName) = 0;
	virtual int getNumOfPlayerGames(std::string userName) = 0;
	virtual std::vector<std::string> getUserNames() = 0;
};