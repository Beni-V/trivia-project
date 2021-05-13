#pragma once

#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"

class LoginRequestHandler;

class RequestHandlerFactory
{
public:
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();

private:
	LoginManager m_loginManager;
	IDAtabase* m_database;
};