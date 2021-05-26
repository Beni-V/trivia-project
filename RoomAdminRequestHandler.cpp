#include "RoomAdminRequestHandler.h"

// will check if request id recieved from user is relevant
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	// return true if request id is relevant and false if its not
	if (requestInfoStruct.requestId == CLOSE_ROOM_REQUEST ||
		requestInfoStruct.requestId == START_GAME_REQUEST ||
		requestInfoStruct.requestId == GET_ROOM_STATE_REQUEST ||
		requestInfoStruct.requestId == LEAVE_ROOM_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}
