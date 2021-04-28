#include "LoginRequestHandler.h"

#define LOGIN_REQUEST "201"

// function will return true if the code message is relevant and false if its not
bool LoignRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	if (requestInfoStruct.requestId == LOGIN_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RequestResult LoignRequestHandler::handleRequest(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	// fill RequestResult struct fields with the informarion in RequestInfo struct
	requestResultStruct.Buffer = requestInfoStruct.buffer;
	requestResultStruct.newHandler = this;
}
