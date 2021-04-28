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
	// abstract class for all request handlers, will be empty in current version(1.0.1)
};