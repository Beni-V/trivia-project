#pragma once

#include <vector>
#include <string>
#include <ctime>

#define LOGIN_REQUEST 201
#define SIGNUP_REQUEST 202
#define CREATE_ROOM_REQUEST 501
#define GET_ROOMS_REQUEST 502
#define GET_PLAYERS_IN_ROOM_REQUEST 503
#define JOIN_ROOM_REQUEST 504
#define GET_STATISTICS_REQUEST 505
#define LOGOUT_REQUEST 506
#define CLOSE_ROOM_REQUEST 507
#define START_GAME_REQUEST 508
#define GET_ROOM_STATE_REQUEST 509
#define LEAVE_ROOM_REQUEST 510
#define SUCCSESS_RESPONSE 1

struct RequestInfo
{
	int requestId;
	time_t receivalTime;
	std::vector<unsigned char> buffer;
};

struct RequestResult;

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