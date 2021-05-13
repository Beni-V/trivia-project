#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
	bool isRequestRelevant(RequestInfo info) override;
	RequestResult handleRequest(RequestInfo info) override;
};