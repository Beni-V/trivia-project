#pragma once
#include <string>

class IDAtabase
{
public:
	virtual bool doesUserExist(std::string userName) = 0;
	virtual bool doesPasswordMatch(std::string userName, std::string password) = 0;
	virtual void assNewUser(std::string userName, std::string password, std::string email) = 0;
};