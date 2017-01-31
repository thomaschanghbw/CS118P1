#include <HTTPMessage.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

// Constructor
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

// Debug
void HTTPMessage::print(){
	cout << _firstLine << endl;

	for (const auto& headerLine : _headerLines) {
		cout << headerLine << endl;
	}

	cout << endl; // blank line

	cout << _entityBody << endl;
}
