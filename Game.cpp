#include "Game.h"

Question Game::getQuestionForUser(LoggedUser user)
{
	return this->m_players[user].currentQuestion;
}
