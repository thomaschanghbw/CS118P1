#ifndef HTTPRESPONSEMESSAGE_INCLUDED
#define HTTPRESPONSEMESSAGE_INCLUDED

#include "HTTPMessage.h"
#include "HTTPRequestMessage.h"
#include <string>

class HTTPResponseMessage: public HTTPMessage {
public:
	// Constructor
	HTTPResponseMessage(const HTTPRequestMessage& requestMessage);

private:
	std::string _statusCode;
};

#endif