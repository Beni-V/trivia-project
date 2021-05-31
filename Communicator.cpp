#define _CRT_SECURE_NO_WARNINGS

#include "Communicator.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#pragma comment(lib, "Ws2_32.lib")

#define MESSAGE_CODE_SIZE 8
#define MESSAGE_LENGTH_SIZE 32
#define MINIMAL_BUFFER_SIZE 40 // minimal message size in bits(message code + message length)
#define SERVER_PORT 5150
#define SUCCESS_LOGIN_STATUS 301
#define AMOUNT_OF_BITS_IN_BYTE 8 
#define BINARY_BASE 2

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
	socketInfo.sin_port = htons(SERVER_PORT);

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
	std::vector<unsigned char> Buffer; // this buffer will store the received message from socket
	int messageCode; // will store the message code of message (first byte)
	int messageLength; // will store the message length of message (next 4 bytes)
	RequestInfo requestInfo; // we will send it to methods in request handler
	time_t currentTime; // will be one of the fields in requestInfo
	std::vector<unsigned char> serializedResponse; // will be the response for received message

	Buffer.resize(MINIMAL_BUFFER_SIZE); // resize the buffer for the first part of message

	// receive the first part of message
	if (recv(newClient, (char*)&Buffer[0], MINIMAL_BUFFER_SIZE, 0) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}

	// get the message code and message length from buffer (this lines are good, i checked it)
	messageCode = std::stoi(std::string(Buffer.begin(), Buffer.end() - MESSAGE_LENGTH_SIZE), 0, BINARY_BASE);
	messageLength = std::stoi(std::string(Buffer.begin() + MESSAGE_CODE_SIZE, Buffer.end()), 0, BINARY_BASE);

	// resize the buffer for the message (message length multiplied by 8 because the message stored in bits)
	Buffer.resize(MINIMAL_BUFFER_SIZE + messageLength * AMOUNT_OF_BITS_IN_BYTE); 

	// receive the rest of the message (i guess that the trouble in  this expression : (char*)(&Buffer + 40))
	if (recv(newClient, (char*)(&Buffer[MINIMAL_BUFFER_SIZE]), messageLength * AMOUNT_OF_BITS_IN_BYTE, 0) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError();
	}

	time(&currentTime); // get current time for field in request info

	// fill requestInfo fields
	requestInfo.buffer = Buffer;
	requestInfo.requestId = messageCode;
	requestInfo.receivalTime = currentTime;

	// if request have irellevant message code
	if (!(this->m_clients.at(newClient)->isRequestRelevant(requestInfo)))
	{
		ErrorResponse errorResponse;
		errorResponse.message = "Irellevant request";
		serializedResponse = JsonResponsePacketSerializer::serializeResponse(errorResponse);
		send(newClient, (char*)&serializedResponse[0], serializedResponse.size(), 0);
	}
	else // if message code is good
	{
		RequestResult response = this->m_clients.at(newClient)->handleRequest(requestInfo);
		send(newClient, (char*)&response.Buffer[0], response.Buffer.size(), 0);
	}
	
}

// will craete a thread for each client socket and add it to map with the needed request handler
void Communicator::startHandleRequests()
{
	SOCKET newClientSocket;
	RequestHandlerFactory* handlerFactory = new RequestHandlerFactory();
	LoginManager* loginManager = new LoginManager();
	IRequestHandler* userRequestHandler = new LoginRequestHandler(*handlerFactory, *loginManager);

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
