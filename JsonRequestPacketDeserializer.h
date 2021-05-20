#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> Buffer);
	static SignupRequest deserializeSignupRequest(std::vector<unsigned char> Buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(std::vector<unsigned char> Buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> Buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> Buffer);
};