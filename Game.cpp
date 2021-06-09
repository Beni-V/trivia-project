#include "Game.h"

Game::Game(std::vector<Question> questions, std::map<LoggedUser, GameData> players) : m_questions(questions), m_players(players)
{
}

Question Game::getQuestionForUser(LoggedUser user)
{
	return this->m_players[user].currentQuestion;
}

void Game::submitAnswer(LoggedUser user, std::string answer)
{
	if (answer == this->m_players[user].currentQuestion.getCorrectAnswer())
	{
		this->m_players[user].correctAnswerCount++;
	}
	else
	{
		this->m_players[user].wrongAnswerCount++;
	}
}

void Game::removePlayer(LoggedUser user)
{
	this->m_players.erase(user);
}
