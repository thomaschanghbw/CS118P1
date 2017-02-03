#include <HTTPRequestMessage.h>
#include <string>
#include <sstream>

using namespace std;

// helpful function declaration
string decodeURL(const string& url);

// Constructor
HTTPRequestMessage::HTTPRequestMessage(const string& buffer)
	:HTTPMessage(buffer) {
	string requestLine = firstLine();
	istringstream iss(requestLine);
	iss >> _method >> _url >> _version;
	_url = decodeURL(_url); // take care of space in url

	for (const auto& hLine : headerLines()) {
		if (hLine.fieldName == "Host:") {
			_host = hLine.value;
		} else if (hLine.fieldName == "Connection:") {
			_connection = hLine.value;
		} else if (hLine.fieldName == "User-agent:") {
			_user_agent = hLine.value;
		} else if (hLine.fieldName == "Accept-language:") {
			_accept_language = hLine.value;
		}
	}
}

// Accessor
string HTTPRequestMessage::method() const {
	return _method;
}

string HTTPRequestMessage::url() const {
	return _url;
}

string HTTPRequestMessage::version() const {
	return _version;
}

string HTTPRequestMessage::Host() const {
	return _host;
}

string HTTPRequestMessage::Connection() const {
	return _connection;
}

string HTTPRequestMessage::User_agent() const {
	return _user_agent;
}

string HTTPRequestMessage::Accept_language() const {
	return _accept_language;
}

// helpful function
string decodeURL(const string& url) {
	string result;
	size_t i = 0;
	while (i < url.size()) {
		if (i+2 < url.size() && url[i] == '%' && url.substr(i+1, 2) == "20") {
			result += ' ';
			i += 3;
		} else {
			result += url[i];
			i += 1;
		}
	}
	return result;
}
