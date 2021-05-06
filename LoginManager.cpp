#include "LoginManager.h"

LoginManager::LoginManager()
{
	this->m_database = new SqliteDatabase();
}

bool LoginManager::signup(std::string userName, std::string password, std::string email)
{
	if (!this->m_database->doesUserExist(userName))
	{
		this->m_database->addNewUser(userName, password, email);
		return true;
	}
	else
	{
		return false;
	}
}

bool LoginManager::login(std::string userName, std::string password)
{
	LoggedUser newUser(userName);
	if (this->m_database->doesPasswordMatch(userName, password))
	{
		this->m_loggedUsers.push_back(newUser);
		return true;
	}
	else
	{
		return false;
	}
}

void LoginManager::logout(std::string userName)
{
	for (auto user = this->m_loggedUsers.begin(); user != this->m_loggedUsers.end(); ++user)
	{
		if (user->getUsername() == userName)
		{
			this->m_loggedUsers.erase(user);
			break;
		}
	}
}
