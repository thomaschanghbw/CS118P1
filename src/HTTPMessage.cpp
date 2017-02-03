#include <HTTPMessage.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

/* headerLine structure */
// Construtor
headerLine::headerLine(const string& line) {
	size_t i = line.find_first_of(' ');
	fieldName = line.substr(0, i);
	value = i != string::npos ? line.substr(i+1) : "";
}

/* HTTPMessage */
// Constructor
HTTPMessage::HTTPMessage() {}

HTTPMessage::HTTPMessage(const string& message) {
	istringstream iss(message);
	getline(iss, _firstLine);

	// reading message until enconter end of file or a black line
	string line;
	while (getline(iss, line)) {
		if (line == "") break;
		headerLine hLine(line); 
		_headerLines.emplace_back(hLine);
	}

	getline(iss, _entityBody);
}

// Accessor
string HTTPMessage::firstLine() const {
	return _firstLine;
}

vector<headerLine> HTTPMessage::headerLines() const {
	return _headerLines;
}

string HTTPMessage::entityBody() const {
	return _entityBody;
}

// Mutator
void HTTPMessage::setFirstLine(const std::string& firstLine) {
	_firstLine = firstLine;
}

void HTTPMessage::addHeaderLine(const headerLine& hLine) {
	_headerLines.emplace_back(hLine);
}

void HTTPMessage::setEntityBody(const std::string& entityBody) {
	_entityBody = entityBody;
}

// Output
string HTTPMessage::to_string() const {
	string result;
	result += _firstLine + "\n";

	for (const auto& hLine : _headerLines) {
		result += hLine.fieldName + " " + hLine.value + "\n";
	}

	if (!_entityBody.empty()) {
		result += "\n"; // blank line
		result += _entityBody;
	}
	return result;
}
