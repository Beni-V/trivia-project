#include "RoomMemberRequestHandler.h"

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;

	this->m_room.removeUser(this->m_user); // remove user from room
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this->m_handlerFactory.createMenuRequestHandler(); // fill newHandler with next handler

	return requestResultStruct;
}

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
