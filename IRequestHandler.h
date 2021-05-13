#pragma once
#include <vector>
#include <string>
#include <ctime>


struct RequestInfo
{
	int requestId;
	time_t receivalTime;
	std::vector<unsigned char> buffer;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
};

struct RequestResult
{
	std::vector<unsigned char> Buffer;
	IRequestHandler* newHandler;
};