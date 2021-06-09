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