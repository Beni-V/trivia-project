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
	std::vector<unsigned char> serializeResponse(LoginResponse LR);
	std::vector<unsigned char> serializeResponse(SignupResponse SR);
	std::vector<unsigned char> serializeResponse(ErrorResponse ER);
};