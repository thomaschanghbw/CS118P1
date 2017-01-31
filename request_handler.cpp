#include "request_handler.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>

const std::string ok = "HTTP/1.1 200 OK";

void Request_handler::handle_request(int sockfd, std::string filename)
{
  std::ifstream requestedFile(filename); // open input file                                                   
  if (!requestedFile) {
    std::cerr << "ERROR, fail to open input file \"" << filename << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string requestedFileContent((std::istreambuf_iterator<char>(requestedFile)), std::istreambuf_iterator<char>());

  //If the request file is valid, create a reply 
  //note: may want to be able to create a reply for invalid files
  m_header_fields.http_status = ok;
  m_header_fields.cont_type = TEXT_HTML;
  m_header_fields.cont_length = requestedFileContent.length();
  std::string header_response = construct_header(m_header_fields);

  //Log response to standard out
  std::cout << header_response << std::endl;
  std::cout << requestedFileContent << std::endl;

  //Send the header followed by the content
  if (write(sockfd, header_response.c_str(), header_response.size()) < 0)
    {
      std::cerr << "ERROR, fail to write to the socket" << std::endl;
      exit(EXIT_FAILURE);
    }
  if (write(sockfd,requestedFileContent.c_str(),requestedFileContent.size()) < 0) {
    std::cerr << "ERROR, fail to write to the socket" << std::endl;
    exit(EXIT_FAILURE);
  }

}

std::string Request_handler::construct_header(Header_fields& header)
{
  std::string header_content = header.http_status + std::string("\r\n") + std::string("Content-Length: ") + 
    std::to_string(header.cont_length) + std::string("\r\n");
  
  //Determine the content-type (currently only support text/html
  switch(header.cont_type)
    {
    case TEXT_HTML:
      header_content += std::string("Content-Type: text/html") + std::string("\r\n");
      break;
    default:
      std::cerr << "Unknown content type";
      exit(1);
    }
  
  //Add extra carriage return linefeed between header and body
  //as per http standard
  header_content += std::string("\r\n");
  return header_content;
    
}
