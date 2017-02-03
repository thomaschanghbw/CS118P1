#ifndef HTTPREQUESTMESSAGE_INCLUDED
#define HTTPREQUESTMESSAGE_INCLUDED

#include <HTTPMessage.h>
#include <string>

class HTTPRequestMessage: public HTTPMessage {
public:
	// Constructor
	HTTPRequestMessage(const std::string& buffer);

	// Accessor
	// request line
	std::string method() const;
	std::string url() const;
	std::string version() const;

	// header line
	std::string Host() const;
	std::string Connection() const;
	std::string User_agent() const;
	std::string Accept_language() const;

private:
	std::string _method;
	std::string _url;
	std::string _version;

	std::string _host;
	std::string _connection;
	std::string _user_agent;
	std::string _accept_language;
};


#endif