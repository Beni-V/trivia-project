#include "Game.h"

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
