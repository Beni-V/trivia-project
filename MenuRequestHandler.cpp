#include "MenuRequestHandler.h"

#define CREATE_ROOM_REQUEST 501
#define GET_ROOMS_REQUEST 502
#define GET_PLAYERS_IN_ROOM_REQUEST 503
#define JOIN_ROOM_REQUEST 504
#define GET_STATISTICS_REQUEST 505
#define LOGOUT_REQUEST 506

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
	// if request id is equal to one of the defined requests id, return true.
	if (info.requestId == CREATE_ROOM_REQUEST ||
		info.requestId == GET_ROOMS_REQUEST ||
		info.requestId == GET_PLAYERS_IN_ROOM_REQUEST ||
		info.requestId == JOIN_ROOM_REQUEST ||
		info.requestId == GET_STATISTICS_REQUEST ||
		info.requestId == LOGOUT_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info)
{
    return RequestResult();
}
