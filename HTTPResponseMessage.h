#ifndef HTTPRESPONSEMESSAGE_INCLUDED
#define HTTPRESPONSEMESSAGE_INCLUDED

#include "HTTPMessage.h"
#include "HTTPRequestMessage.h"
#include <string>

class HTTPResponseMessage: public HTTPMessage {
public:
	// Constructor
	HTTPResponseMessage(const HTTPRequestMessage& requestMessage);

	// Accessor
	std::string Connection() const;

private:
	std::string _statusCode;
	std::string _connection;
};

#endif