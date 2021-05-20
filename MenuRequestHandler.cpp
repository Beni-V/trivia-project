#include "MenuRequestHandler.h"

#define CREATE_ROOM_REQUEST 501
#define GET_ROOMS_REQUEST 502
#define GET_PLAYERS_IN_ROOM_REQUEST 503
#define JOIN_ROOM_REQUEST 504
#define GET_STATISTICS_REQUEST 505
#define LOGOUT_REQUEST 506
#define SUCCSESS_RESPONSE 1

MenuRequestHandler::MenuRequestHandler(RoomManager& roomManager, StatisticsManager& statisticsManager, RequestHandlerFactory& handlerFactory): m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_handlerFactory(handlerFactory)
{
}

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

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	RequestResult requestResultStruct;

	this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername()); // logout user
	requestResultStruct.newHandler = this->m_handlerFactory.createLoginRequestHandler(); // fill newHandler with next handler (which is login)
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ SUCCSESS_RESPONSE }); // fill Buffer with serialized logout response
	return requestResultStruct;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	RequestResult requestResultStruct;

	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ SUCCSESS_RESPONSE }); // fill Buffer with serialized get rooms response
	return requestResultStruct;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetPlayersInRoomResponse response = {this->m_roomManager.getRooms()[JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer).roomId].getAllUsers()}; // create response with users in room
	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetPersonalStatsResponse response = { SUCCSESS_RESPONSE, this->m_statisticsManager.getUserStatistics(this->m_user.getUsername()) }; // cerate response with user stats
	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	RequestResult requestResultStruct;

	GetHighScoreResponse response = { SUCCSESS_RESPONSE, this->m_statisticsManager.getHighScore() }; // create response with top scored users
	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler
	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(response); // fill buffer with serialized response

	return requestResultStruct;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer); // deserialize request
	this->m_roomManager.getRooms()[request.roomId].addUser(this->m_user); // add user to room

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ SUCCSESS_RESPONSE }); // fill buffer with serialized response
	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler

	return requestResultStruct;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	RequestResult requestResultStruct;

	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer); // deserialize request
	this->m_roomManager.createRoom(this->m_user, RoomData{(unsigned int)(this->m_roomManager.getRooms().size() + 1), request.roomName, request.maxUsers, request.questionCount, request.answerTimeout, false}); // create room

	requestResultStruct.Buffer = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ SUCCSESS_RESPONSE });// fill buffer with serialized response
	requestResultStruct.newHandler = new MenuRequestHandler(this->m_roomManager, this->m_statisticsManager, this->m_handlerFactory); // fill newHandler with next handler

	return requestResultStruct;
}


