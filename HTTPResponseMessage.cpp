#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"
#include <string>
#include <unordered_map>
#include <iostream>		 // for I/O
#include <fstream>       // for file I/O
#include <streambuf>     // for converting file stream to string
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

const string HTTPVERSION = "HTTP/1.1";

std::unordered_map<std::string, std::string> statusCode_phrase_map ({
	{"200", "OK"},
	{"301", "Moved Permanently"},
	{"400", "Bad Request"},
	{"404", "Not Found"},
	{"505", "HTTP Version Not Supported"}
});

// helpful function
string getFileExtension(const string& fileName) {
	size_t i = fileName.find_last_of('.');
	if (i != string::npos) 
		return fileName.substr(i+1);
	return "";
}

string getCurrentTime() {
  	time_t rawtime;
  	struct tm * timeinfo;

  	time(&rawtime);
  	timeinfo = localtime(&rawtime);
  	string result(asctime(timeinfo));
  	result.resize(result.size()-1); // stip the new line at the end
  	return result; 
}

string getLastModifiedTime(const string& fileName) {
	struct stat st;
	stat(fileName.c_str(), &st);

	struct tm * timeinfo;
  	timeinfo = localtime(&st.st_mtime);
  	string result(asctime(timeinfo));
  	result.resize(result.size()-1); // stip the new line at the end
  	return result; 
}

HTTPResponseMessage::HTTPResponseMessage(const HTTPRequestMessage& requestMessage) {
	if (requestMessage.version() != HTTPVERSION) {
		_statusCode = "505";
	}

	if (_statusCode.empty() && requestMessage.method() != "GET") {
		_statusCode = "400";
		cerr << "Request Method: " << requestMessage.method() << " not implemented!" << endl; 
	}

	string fileName = "." + requestMessage.url();
	ifstream requestedFile(fileName); // open input file
    if (_statusCode.empty() && !requestedFile) {
    	_statusCode = "404";
      	cerr << "ERROR, fail to open input file \"" << fileName << "\"" << endl;
    }

    if (_statusCode.empty()) {
    	_statusCode = "200";
    }

    // set status line 
    string statusLine = HTTPVERSION + " " + _statusCode + " " + statusCode_phrase_map[_statusCode];
    setFirstLine(statusLine);

    // set entity body
    string requestedFileContent((istreambuf_iterator<char>(requestedFile)), istreambuf_iterator<char>());
    setEntityBody(requestedFileContent);

    // set header lines
    string tmp;
    // connection
    if (requestMessage.Connection() == "close") {
    	_connection = "close";
    	addHeaderLine(headerLine("Connection: close"));
    } else {
    	_connection = "keep-alive";
    	addHeaderLine(headerLine("Connection: keep-alive")); // http by default implements persistent connection
    }
    // date
    tmp = "Date: " + getCurrentTime();
    addHeaderLine(headerLine(tmp));

    // server
    addHeaderLine(headerLine("Server: Webserver/1.0"));

    // last modified
    if (_statusCode == "200") {
    	tmp = "Last-Modified: " + getLastModifiedTime(fileName);
    	addHeaderLine(tmp);
    }

    // content-length
    tmp = "Content-Length: " + std::to_string(requestedFileContent.size());
    addHeaderLine(headerLine(tmp));

    // content type
    if (_statusCode == "200" && getFileExtension(fileName) == "html") {
    	addHeaderLine(headerLine("Content-Type: text/html"));
    }

} // automatically close the file stream

// Accessor
string HTTPResponseMessage::Connection() const {
	return _connection;
}

