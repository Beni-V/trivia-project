#include "RoomMemberRequestHandler.h"

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo requestInfoStruct)
{
	if (requestInfoStruct.requestId == LEAVE_ROOM_REQUEST || requestInfoStruct.requestId == GET_ROOM_STATE_REQUEST)
	{
		return true;
	}
	else
	{
		return false;
	}
}
