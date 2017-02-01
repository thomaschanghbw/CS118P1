#ifndef HTTPMESSAGE_INCLUDED
#define HTTPMESSAGE_INCLUDED 

#include <string>
#include <vector>

class HTTPMessage {
public:
	// Constructor
	HTTPMessage();
	HTTPMessage(const std::string& message);

	// Accessor
	std::string firstLine() const;
	std::vector<std::string> headerLines() const;
	std::string entityBody() const;

	// Mutator
	void setFirstLine(const std::string& firstLine);
	void addHeaderLine(const std::string& headerLine);
	void setEntityBody(const std::string& entityBody);

	// output
	std::string to_string() const;

private:
	std::string _firstLine;
	std::vector<std::string> _headerLines;
	std::string _entityBody;
};

#endif