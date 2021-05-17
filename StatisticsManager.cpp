#include "StatisticsManager.h"

std::vector<std::string> StatisticsManager::getHighScore()
{
	std::vector<std::string> topPlayers;
	int currPlayerTotalAnswers;
	int currPlayerRightAnswers;
	std::string currPlayerUserName;

}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string userName)
{
	// gte the stats from DB
	std::string playerAvegageAnswerTime = std::to_string(this->m_database->getPlayerAverageAnswerTime(userName));
	std::string numOfCorrectAnswers = std::to_string(this->m_database->getNumOfCorrectAnswers(userName));
	std::string numOfTotalAnswers = std::to_string(this->m_database->getNumOfTotalAnswers(userName));
	std::string numOfPlayerGames = std::to_string(this->m_database->getNumOfPlayerGames(userName));

	// return the stats in vector
	return std::vector<std::string>({ playerAvegageAnswerTime , numOfCorrectAnswers, numOfTotalAnswers, numOfPlayerGames });
}
