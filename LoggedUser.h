#pragma once
#include <string>
class LoggedUser
{
private:
	std::string m_username;

public:
	LoggedUser();
	LoggedUser(std::string username);
	std::string getUsername();
	bool operator==(const LoggedUser& user) const;
};