#pragma once
#include <map>
#include "Question.h"
#include "LoggedUser.h"

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;

public:
	Question getQuestionForUser(LoggedUser user);
	void submitAnswer(LoggedUser user, std::string answer);
	void removePlayer(LoggedUser user);
};