#ifndef HTTPREQUESTMESSAGE_INCLUDED
#define HTTPREQUESTMESSAGE_INCLUDED

#include <HTTPMessage.h>
#include <string>

class HTTPRequestMessage: public HTTPMessage {
public:
	HTTPRequestMessage(const string& buffer);
	std::string method() const;
	std::string url() const;
	std::string version() const;

private:
	std::string _method;
	std::string _url;
	std::string _version;
};


#endif