#include <HTTPMessage.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

// Constructor
HTTPMessage::HTTPMessage() {}

HTTPMessage::HTTPMessage(const string& message) {
	istringstream iss(message);
	getline(iss, _firstLine);

	// reading message until enconter end of file or a black line
	string line;
	while (getline(iss, line)) {
		if (line == "") break;
		_headerLines.emplace_back(line);
	}

	getline(iss, _entityBody);
}

// Accessor
string HTTPMessage::firstLine() const {
	return _firstLine;
}

vector<string> HTTPMessage::headerLines() const {
	return _headerLines;
}

string HTTPMessage::entityBody() const {
	return _entityBody;
}

// Mutator
void HTTPMessage::setFirstLine(const std::string& firstLine) {
	_firstLine = firstLine;
}

void HTTPMessage::addHeaderLine(const std::string& headerLine) {
	_headerLines.emplace_back(headerLine);
}

void HTTPMessage::setEntityBody(const std::string& entityBody) {
	_entityBody = entityBody;
}

// Output
string HTTPMessage::to_string() const {
	string result;
	result += _firstLine + "\n";

	for (const auto& headerLine : _headerLines) {
		result += headerLine + "\n";
	}

	if (!_entityBody.empty()) {
		result += "\n"; // blank line
		result += _entityBody;
	}
	return result;
}
