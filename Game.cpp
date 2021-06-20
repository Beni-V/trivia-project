#include "Game.h"

Game::Game(std::vector<Question> questions, std::map<std::string, GameData> players) : m_questions(questions), m_players(players)
{
}

Question Game::getQuestionForUser(std::string user)
{
    return this->m_players.at(user).currentQuestion;
}

void Game::submitAnswer(std::string user, int answerId)
{
    for (int i = 0; i < this->m_questions.size(); i++)
    {
        if (this->m_questions[i] == this->m_players.at(user).currentQuestion && i != this->m_questions.size())
        {
            this->m_players.at(user).currentQuestion = this->m_questions[i + 1];
            break;
        }
    }
    if (answerId == CORRECT_ANSWER_INDEX)
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

std::vector<PlayerResult> Game::getPlayersResults()
{
    std::vector<PlayerResult> results;

    for (std::pair<std::string, GameData> player : this->m_players)
    {
        results.push_back(PlayerResult{ player.first, player.second.correctAnswerCount, player.second.wrongAnswerCount, 0 });
    }

    return results;
}
