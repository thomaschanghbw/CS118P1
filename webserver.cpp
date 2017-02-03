#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in

#include <cstdlib>       // definitions of exit status
#include <cstring>       // for string manipulation
#include <string>        // for string manipulation
#include <iostream>      // for I/O

#include <HTTPRequestMessage.h>
#include <HTTPResponseMessage.h>

using namespace std;

void dieWithError(const string& errorMsg) {
    cerr << errorMsg << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2) {
        dieWithError("ERROR, please provide one and only one argument to the program.");
    }

    int portno = stoi(argv[1]);
    if (portno < 1 || portno > 65535) { // 0 is reserved
        dieWithError("ERROR, please provide the correct port number.");
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
        dieWithError("ERROR, fail to open socket");
    }

    // assign the local ip address and the port number to the newly created socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        dieWithError("ERROR, fail to bind");
    }
   
    // listen for TCP connection from the client
    listen(sockfd, 1);
  
    socklen_t clilen = sizeof(cli_addr);
  
    while (1) {
        // create a connection socket dedicated to this particular client
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            dieWithError("ERROR, fail to accept");
        }
       
        char buffer[512];
        memset(buffer, 0, 512);
        if (read(newsockfd, buffer, 511) < 0) {
            dieWithError("ERROR, fail to read from the socket");
        }

        //dump to the console, need to think about the case when we buffer size is not enough, eg. very long URL
        cout << buffer << endl; 
        HTTPRequestMessage requestM(buffer); // constructing request message
        HTTPResponseMessage reponseM(requestM); // constructing response message
        string messageToSend = reponseM.to_string();
        // cout << messageToSend << endl; // debug

        if (write(newsockfd,messageToSend.c_str(),messageToSend.size()) < 0) {
            dieWithError("ERROR, fail to write to the socket");
        }

        if (reponseM.Connection() == "close") {
            close(newsockfd);
        } else {
            // implement time out
            close(newsockfd);
        }
    }
  
    return 0; // we never get here since the server is always on.
}
