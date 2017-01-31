#ifndef HTTPRESPONSEMESSAGE_INCLUDED
#define HTTPRESPONSEMESSAGE_INCLUDED

#include "HTTPRequestMessage.h"
#include <string>

class HTTPResponseMessage {
public:
	HTTPResponseMessage(const HTTPRequestMessage& requestMessage);

private:
	std::string _version;
	std::string _statusCode;
	std::string _phrase;
};

#endif