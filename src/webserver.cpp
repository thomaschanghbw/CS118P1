#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <unistd.h>      // close()
#include <sys/wait.h>    // waitpid()

#include <cstdlib>       // definitions of exit status
#include <cstring>       // for string manipulation
#include <string>        // for string manipulation
#include <iostream>      // for I/O

#include <HTTPRequestMessage.h>
#include <HTTPResponseMessage.h>

using namespace std;

const size_t MAXPENDING = 5; // maximum outstanding connection requests
const size_t BUFFER_SIZE = 8192;

// utility function declaration
void dieWithError(const string& errorMsg);
void perClientProcess(int portno);
void constraintMultitasking(int portno, int maxNumProcess);
int createTCPListeningSocket(int portno);     // Create a listening socket to listen for client
int acceptTCPConnection(int listeningSocket); // Create a connection socket dedicated to this particular client
void handleTCPClient(int socket);

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        dieWithError("ERROR, please provide two or three argument to the program. <port number> (<maxNumProcess>)");
    }

    int portno = stoi(argv[1]);
    if (portno < 1 || portno > 65535) { // 0 is reserved
        dieWithError("ERROR, please provide the correct port number.");
    }

    if (argc == 2) {
        perClientProcess(portno);
    } else { // argc == 3
        int maxNumProcess = stoi(argv[2]);
        if (maxNumProcess <= 0) {
            dieWithError("ERROR, please provide positive max number of process.");
        }
        constraintMultitasking(portno, maxNumProcess);
    }

    return 0;
}

void perClientProcess(int portno) {
    int listeningSocket = createTCPListeningSocket(portno);
    unsigned int numChildProcess = 0;
    while (1) {
        int connectionSocket = acceptTCPConnection(listeningSocket);
        pid_t processId = fork();
        if (processId < 0) {
            dieWithError("ERROR, failed to fork");
        } else if (processId == 0) { // child process
            close(listeningSocket);
            handleTCPClient(connectionSocket);
            exit(EXIT_SUCCESS);
        }
        
        // parent process
        close(connectionSocket); // parent do not handle clients
        ++numChildProcess;

        // Clean up zombie process
        while (numChildProcess) {
            processId = waitpid((pid_t) -1, NULL, WNOHANG); // Nonblocking wait
            if (processId < 0) {
                dieWithError("ERROR, fail to reap zombie in waitpid()");
            } else if (processId == 0) { // no zombie
                break;
            } else {
                --numChildProcess;
            }
        }
    }
    // we never get here since the server is always on.
}

void constraintMultitasking(int portno, int maxNumProcess) {
    int listeningSocket = createTCPListeningSocket(portno);
    for (int i = 0; i < maxNumProcess; ++i) {
        pid_t processId = fork();
        if (processId < 0) {
            dieWithError("ERROR, failed to fork");
        } else if (processId == 0) { // child process
            while (1) {
                int connectionSocket = acceptTCPConnection(listeningSocket);
                handleTCPClient(connectionSocket);
            }
        }
    }
    exit(EXIT_SUCCESS); // we do not need parent any more
}

void dieWithError(const string& errorMsg) {
    cerr << errorMsg << endl;
    exit(EXIT_FAILURE);
}

int createTCPListeningSocket(int portno) {
    // declare and initialize server address
    struct sockaddr_in serv_addr;
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
    if (listen(sockfd, MAXPENDING) < 0) {
        dieWithError("ERROR, fail in listen()");
    }

    return sockfd;
}

int acceptTCPConnection(int listeningSocket) {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(listeningSocket, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        dieWithError("ERROR, fail to accept");
    }   

    return newsockfd;
}

void handleTCPClient(int socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int recvMsgSize = recv(socket, buffer, BUFFER_SIZE-1, 0); // terminate the buffer with the last byte
    if (recvMsgSize < 0) {
        dieWithError("ERROR, fail to receive message from the socket");
    }

    if (recvMsgSize == (BUFFER_SIZE-1)) {
        cerr << "WARNING, request message received is at least " << to_string(BUFFER_SIZE-1)
             << "bytes! Maybe malicious!" << endl;
    }

    // dump the request to the terminal
    cout << buffer << endl; 
    HTTPRequestMessage requestM(buffer); // constructing request message from the buffer
    HTTPResponseMessage reponseM(requestM); // constructing response message from the request message
    string messageToSend = reponseM.to_string();
    // cout << messageToSend << endl; // debug

    int nBytesSent = send(socket, messageToSend.c_str(), messageToSend.size(), 0);
    if (nBytesSent < 0 || (unsigned int) nBytesSent != messageToSend.size()) {
        dieWithError("ERROR, fail to write to the socket");
    }

    if (reponseM.Connection() == "close") {
        close(socket);
    } else {
        // implement time out
        close(socket);
    }
}