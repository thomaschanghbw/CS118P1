class HTTPRequestMessage {
public:
	HTTPRequestMessage(const string& buffer) {
		
	}

	string requestLine() const;

private:
	string _requestLine;
}