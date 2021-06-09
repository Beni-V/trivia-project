#pragma once
#include <vector>
#include <string>
#include <ctime>

#define LOGIN_REQUEST 201
#define SIGNUP_REQUEST 202
#define CREATE_ROOM_REQUEST 91
#define GET_ROOMS_REQUEST 92
#define GET_PLAYERS_IN_ROOM_REQUEST 93
#define JOIN_ROOM_REQUEST 94
#define GET_STATISTICS_REQUEST 95
#define LOGOUT_REQUEST 96
#define CLOSE_ROOM_REQUEST 97
#define START_GAME_REQUEST 98
#define GET_ROOM_STATE_REQUEST 99
#define LEAVE_ROOM_REQUEST 100
#define GET_HIGH_SCORES_REQUEST 101
#define GET_GAME_RESULT_REQUEST 110
#define SUBMIT_ANSWER_REQUEST 111
#define GET_QUESTION_REQUEST 112
#define LEAVE_GAME_REQUEST 113
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