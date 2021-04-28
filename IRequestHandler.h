#pragma once
#include <vector>
#include <string>
#include <ctime>

struct RequestResult
{
	std::vector<unsigned char> Buffer;
	IRequestHandler* newHandler;
};

struct RequestInfo
{
	std::string requestId;
	time_t receivalTime;
	std::vector<unsigned char> buffer;
};

class IRequestHandler
{
	virtual bool isRequestRelevant(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
};