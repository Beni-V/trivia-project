#pragma once
#include "SqliteDatabase.h"

class StatisticsManager
{
private:
	IDAtabase* m_database;
public:
	StatisticsManager();
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string userName);
};