#include "HTTPResponseMessage.h"
#include "HTTPRequestMessage.h"
#include <string>
#include <unordered_map>
#include <iostream>		 // for I/O
#include <fstream>       // for file I/O
#include <streambuf>     // for converting file stream to string
#include <iterator>
#include <algorithm>
#include <ctime>
#include <cctype>

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

// helpful function declaration
string getFileExtension(const string& fileName);
string getCurrentTime();
string getLastModifiedTime(const string& fileName);
string htmlPage(const string& message);

HTTPResponseMessage::HTTPResponseMessage(const HTTPRequestMessage& requestMessage) {
	if (requestMessage.version() != HTTPVERSION) {
		_statusCode = "505";
	}

	if (_statusCode.empty() && requestMessage.method() != "GET") {
		_statusCode = "400";
		cerr << "Request Method: " << requestMessage.method() << " not implemented!" << endl; 
	}

	// if client doesn't specify file name, direct it to index.html
	string fileName = requestMessage.url() == "/" ? "./index.html" : "." + requestMessage.url();
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

    string tmp;
    // set entity body
    if (_statusCode == "200") {
    	tmp = string((istreambuf_iterator<char>(requestedFile)), istreambuf_iterator<char>());
    } else {
    	tmp = htmlPage(_statusCode + ": " + statusCode_phrase_map[_statusCode] + "!");
    }
    setEntityBody(tmp);

    // set header lines
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
    tmp = "Content-Length: " + std::to_string(entityBody().size());
    addHeaderLine(headerLine(tmp));

    // content type
    if (_statusCode == "200") {
    	string ext = getFileExtension(fileName);
    	string ext_low;
    	std::transform(ext.begin(), ext.end(), back_inserter(ext_low), ::tolower);
    	if (ext_low == "html") {
    		addHeaderLine(headerLine("Content-Type: text/html"));
    	} else if (ext_low == "gif") {
    		addHeaderLine(headerLine("Content-Type: img/gif"));
    	} else if (ext_low == "jpeg" || ext_low == "jpg") {
    		addHeaderLine(headerLine("Content-Type: img/jpeg"));
    	} else if (ext_low == "ico") {
    		addHeaderLine(headerLine("Content-Type: img/x-icon"));
    	}
    }

} // automatically close the file stream

// Accessor
string HTTPResponseMessage::Connection() const {
	return _connection;
}

// helpful functions
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

string htmlPage(const string& message) {
	string result = "<!DOCTYPE html>\n<html><body><h1>" + message + "</h1></body></html>";
	return result;
}
