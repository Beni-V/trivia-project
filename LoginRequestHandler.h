#pragma once
#include "IRequestHandler.h"

class LoignRequestHandler : public IRequestHandler
{
	bool isRequestRelevant(RequestInfo requestInfoStruct) override;
	RequestResult handleRequest(RequestInfo requestInfoStruct) override;
};