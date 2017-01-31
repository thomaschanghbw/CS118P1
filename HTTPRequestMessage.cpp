#include <HTTPRequestMessage.h>
#include <string>
#include <sstream>

using namespace std;

// Constructor
HTTPRequestMessage::HTTPRequestMessage(const string& buffer)
	:HTTPMessage(buffer) {
	string requestLine = firstLine();
	istringstream iss(requestLine);
	iss >> _method >> _url >> _version;
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