#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

class Request_handler
{
 public:
  void handle_request(int sockfd, std::string filename);

 private:
  enum Content_type
  {
    TEXT_HTML = 0,
    TEXT_PLAIN = 1
  };

  struct Header_fields
  {
    std::string http_status;
    Content_type cont_type;
    int cont_length;
  };
  Header_fields m_header_fields;

  std::string construct_header(Header_fields& header);
};


#endif //REQUEST_HANDLER_H
