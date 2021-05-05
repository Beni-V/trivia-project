#pragma once
#include "json.hpp"
#include <string>
#include <vector>
#include <bitset>

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string message;
};

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(LoginResponse LR);
	static std::vector<unsigned char> serializeResponse(SignupResponse SR);
	static std::vector<unsigned char> serializeResponse(ErrorResponse ER);
};