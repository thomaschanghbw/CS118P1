#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in

#include <cstdlib>       // definitions of exit status
#include <cstring>       // for string manipulation
#include <string>        // for string manipulation
#include <iostream>      // for I/O
#include <fstream>       // for file I/O
#include <streambuf>     // for converting file stream to string
#include <unistd.h>
#include "request_handler.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2 || argc > 2) {
    cerr << "ERROR, please provide one and only one argument to the program." << endl;
    exit(EXIT_FAILURE);
  }

  int portno = stoi(argv[1]);
  if (portno < 0 || portno > 65536) {
    cerr << "ERROR, please provide the correct port number." << endl;
    exit(EXIT_FAILURE);
  }

  // declare and initialize server address
  struct sockaddr_in serv_addr, cli_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno); // convert to net order byte

  // create a TCP socket (with underlying network is using IPv4)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    cerr << "ERROR, fail to open socket" << endl;
    exit(EXIT_FAILURE);
  }

  // assign the local ip address and the port number to the newly created socket
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    cerr << "ERROR, fail to bind" << endl;
    exit(EXIT_FAILURE);
  }
   
  // listen for TCP connection from the client
  listen(sockfd, 1);
  
  socklen_t clilen = sizeof(cli_addr);
  
  while (1) {
    // create a connection socket dedicated to this particular client
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      cerr << "ERROR, fail to accept" << endl;
      exit(EXIT_FAILURE);
    }
       
    char buffer[512];
    memset(buffer, 0, 512);
    if (read(newsockfd, buffer, 511) < 0) {
      cerr << "ERROR, fail to read from the socket" << endl;
      exit(EXIT_FAILURE);
    }
    cout << buffer << endl; //dump to the console, need to think about the case when we buffer size is not enough, eg. very long URL

    // testing file for now, need to extract the file path from request message
    string filename = "index.html";
    
    Request_handler server_request_handler;
    server_request_handler.handle_request(newsockfd, filename);

    close(newsockfd);
  } // automatically close the file stream
  
  return 0; // we never get here since the server is always on.
}
