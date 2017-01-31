#ifndef HTTPMESSAGE_INCLUDED
#define HTTPMESSAGE_INCLUDED 

#include <string>
#include <vector>

class HTTPMessage {
public:
	// Constructor
	HTTPMessage(const std::string& message);

	// Accessor
	std::string firstLine() const;
	std::vector<std::string> headerLines() const;
	std::string entityBody() const;

	// Debug
	void print();

private:
	std::string _firstLine;
	std::vector<std::string> _headerLines;
	std::string _entityBody;
};

#endif