#include "LoginRequestHandler.h"

#define LOGIN_REQUEST "201"

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
