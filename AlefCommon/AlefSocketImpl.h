#pragma once

#include "Poco/Net/StreamSocketImpl.h"

using namespace Poco::Net;

#include "AlefSocket.h"

class AlefSocketImpl : public StreamSocketImpl
{
public:
	AlefSocketImpl() {};
	virtual ~AlefSocketImpl() {};
};