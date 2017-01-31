#ifndef HTTPREQUESTMESSAGE_INCLUDED
#define HTTPREQUESTMESSAGE_INCLUDED

#include <HTTPMessage.h>
#include <string>

class HTTPRequestMessage: public HTTPMessage {
public:
	// Constructor
	HTTPRequestMessage(const std::string& buffer);

	// Accessor
	std::string method() const;
	std::string url() const;
	std::string version() const;

private:
	std::string _method;
	std::string _url;
	std::string _version;
};


#endif