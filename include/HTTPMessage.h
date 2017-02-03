#ifndef HTTPMESSAGE_INCLUDED
#define HTTPMESSAGE_INCLUDED 

#include <string>
#include <vector>

struct headerLine {
	headerLine(const std::string& line);
	std::string fieldName;
	std::string value;
};

class HTTPMessage {
public:
	// Constructor
	HTTPMessage();
	HTTPMessage(const std::string& message);

	// Accessor
	std::string firstLine() const;
	std::vector<headerLine> headerLines() const;
	std::string entityBody() const;

	// Mutator
	void setFirstLine(const std::string& firstLine);
	void addHeaderLine(const headerLine& hLine);
	void setEntityBody(const std::string& entityBody);

	// output
	std::string to_string() const;

private:
	std::string _firstLine;
	std::vector<headerLine> _headerLines;
	std::string _entityBody;
};

#endif