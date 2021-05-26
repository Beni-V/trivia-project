#include "RoomAdminRequestHandler.h"

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	this->m_roomManager.deleteRoom(this->m_room.getId());

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse{SUCCSESS_RESPONSE}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo requestInfoStrust)
{
	RequestResult requestResultStruct;

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ SUCCSESS_RESPONSE, this->m_room.getIsActive(), this->m_room.getAllUsers(), this->m_room.getQuestionsAmount(), this->m_room.getQuestionTimeOut()}); // fill buffer with serialized response
	requestResultStruct.newHandler = this; // fill newHandler with next handler

	return requestResultStruct;
}

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
