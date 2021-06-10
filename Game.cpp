#include "Game.h"

Game::Game(std::vector<Question> questions, std::map<std::string, GameData> players) : m_questions(questions), m_players(players)
{
}

Question Game::getQuestionForUser(std::string user)
{
    return this->m_players.at(user).currentQuestion;
}

void Game::submitAnswer(std::string user, std::string answer)
{
    if (answer == this->m_players.at(user).currentQuestion.getCorrectAnswer())
    {
        this->m_players.at(user).correctAnswerCount++;
    }
    else
    {
        this->m_players.at(user).wrongAnswerCount++;
    }
}

void Game::removePlayer(std::string user)
{
    this->m_players.erase(user);
}