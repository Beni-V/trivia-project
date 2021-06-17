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
    std::map<std::string, GameData> m_players;

public:
    Game(std::vector<Question> questions, std::map<std::string, GameData> players);
    Question getQuestionForUser(std::string user);
    void submitAnswer(std::string user, int answerId);
    void removePlayer(std::string user);
};