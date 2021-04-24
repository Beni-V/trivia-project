#include "Communicator.h"
#pragma comment(lib, "Ws2_32.lib")

#define DATA_LEN 5

// this function will set up the server socket and start listening ofr connections
void Communicator::bindAndListen()
{
	struct sockaddr_in socketInfo; 

	// Initialize Winsock
	int iResult;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << WSAGetLastError();
	}

	// create the server socket
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// check if socket created without troubles
	if (m_serverSocket == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError();
	}
	else
	{
		std::cout << "Starting..." << std::endl;
	}

	// Set up the socketInfo structure
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = INADDR_ANY;
	socketInfo.sin_port = htons(5150);

	// bind server socket, and print error if wasnt successfull
	if (bind(this->m_serverSocket, (SOCKADDR*)&socketInfo, sizeof(socketInfo)) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}
	else 
	{
		std::cout << "Server socket binded\n";
	}

	// listen server socket, and print error if wasnt successfull
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}
	else
	{
		std::cout << "Server socket listening\n";
	}
}

void Communicator::handleNewClients(SOCKET newClient)
{
	char RecievedData[DATA_LEN];

	// sent the message "Hello"
	if (send(newClient, "Hello", DATA_LEN, 0) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}
	else
	{
		std::cout << "Hello message sent to the new client!\n";
	}

	// Recieve message with the given maximum length
	if (recv(newClient, RecievedData, DATA_LEN, 0) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}
	
	// print the recieved data char by char (cause there is no space for null in the end)
	std::cout << "Recieved message from client: ";
	for (int i = 0; i < DATA_LEN; i++)
	{
		std::cout << RecievedData[i];
	}
	std::cout << std::endl;
}

// will craete a thread for each client socket and add it to map with the needed request handler
void Communicator::startHandleRequests()
{
	SOCKET newClientSocket;
	IRequestHandler* userRequestHandler = new LoignRequestHandler;

	bindAndListen();

	// accept clients forever
	while (true)
	{
		newClientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (newClientSocket == INVALID_SOCKET)
		{
			std::cout << WSAGetLastError();
		}
		else
		{
			std::cout << "New client accepted\n";
		}

		// create thread for each client
		std::thread clientThread(&Communicator::handleNewClients, this, newClientSocket);
		clientThread.detach();

		this->m_clients.insert({ newClientSocket, userRequestHandler}); // add client to map (class field) with his request handler
	}
}
