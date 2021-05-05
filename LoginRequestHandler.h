#pragma once
#include "IRequestHandler.h"

class LoignRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(RequestInfo requestInfoStruct) override;
	RequestResult handleRequest(RequestInfo requestInfoStruct) override;
};