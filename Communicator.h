#pragma once
#include <winsock2.h>
#include <map>
#include <iostream>
#include <thread>
#include "LoginRequestHandler.h"
#include "IRequestHandler.h"

class Communicator
{
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;

	void bindAndListen();
	void handleNewClients(SOCKET newClient, LoginRequestHandler userLoginRequest);

public:
	void startHandleRequests();
};