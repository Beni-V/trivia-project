#pragma once
#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include <algorithm>

class LoginManager
{
private:
	IDAtabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

public:
	LoginManager();
	bool signup(std::string userName, std::string password, std::string email);
	bool login(std::string userName, std::string password);
	void logout(std::string userName);
};