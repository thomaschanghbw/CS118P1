#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"
#include <string>
#include <unordered_map>
#include <iostream>		 // for I/O
#include <fstream>       // for file I/O
#include <streambuf>     // for converting file stream to string

using namespace std;

const string HTTPVERSION = "HTTP/1.1";

std::unordered_map<std::string, std::string> statusCode_phrase_map ({
	{"200", "OK"},
	{"301", "Moved Permanently"},
	{"400", "Bad Request"},
	{"404", "Not Found"},
	{"505", "HTTP Version Not Supported"}
});

HTTPResponseMessage::HTTPResponseMessage(const HTTPRequestMessage& requestMessage) {
	if (requestMessage.version() != HTTPVERSION) {
		_statusCode = "505";
	}

	if (requestMessage.method() != "GET") {
		_statusCode = "400";
		cerr << "Request Method: " << requestMessage.method() << " not implemented!" << endl; 
	}

	string fileName = "." + requestMessage.url();
	ifstream requestedFile(fileName); // open input file
    if (!requestedFile) {
    	_statusCode = "404";
      	cerr << "ERROR, fail to open input file \"" << fileName << "\"" << endl;
    }

    if (_statusCode.empty()) {
    	_statusCode = "200";
    }

    string responseHeadLine = HTTPVERSION + " " + _statusCode + " " + statusCode_phrase_map[_statusCode];
    setFirstLine(responseHeadLine);

    string requestedFileContent((istreambuf_iterator<char>(requestedFile)), istreambuf_iterator<char>());
    setEntityBody(requestedFileContent);
} // automatically close the file stream

