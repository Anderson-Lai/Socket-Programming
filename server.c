#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> // contains data types for sys calls
#include <sys/socket.h> // defintions for socket structures
#include <netinet/in.h> // needed for internet domain addresses
#include <unistd.h> // functions like close()
#include <strings.h> // for bzero() -> sets the first n value in ptr to 0
#include <string.h>

#define nullptr NULL

// socket() -> create a socket
// bind() -> bind the created socket to a port and ip
// listen() -> well, listen
// accept() -> accept read / write requests
// read(), write() -> read / write
// close() -> close the socket and all connections

// connect() -> (client side), connect to the server

void error (const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    
    if (argc < 2) {
        fprintf(stderr, "Port is not provided. Program terminated\n");
        exit(EXIT_FAILURE);
    }

    int sockfd, newsockfd, portno, n; // socket, new socket, port number, checking for failures
    char buffer[255]; // buffer to send messages; save msgs to buffer then send it

    struct sockaddr_in server_addr, cli_addr; // base struct for ipv4 communications
    socklen_t clilen; // 32 bit data type

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // ipv4, tcp, default settings
    if (sockfd < 0) { // returns -1 if there is a failure
        error("Error opening socket.");
    }

    bzero((char*)& server_addr, sizeof(server_addr)); // clears any data 
    portno = atoi(argv[1]);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // sets the ip address to be any that the machine has access to
    server_addr.sin_port = htons(portno); // host to network short (the data type (e.g. int, long))
    // htons orders bytes in highest->lowest order; this is tcp standard

    // sockaddr is a more general struct than sockaddr_in
    // sockaddr_in "inherits" from sockaddr
    if (bind(sockfd, (struct sockaddr*)& server_addr, sizeof(server_addr)) < 0) {
        error("Binding failed.\n");
    }

    listen(sockfd, 5); // maximum number of connections to enqueue
    clilen = sizeof(cli_addr); // cli_addr is not 0; it is the size of the struct

    // new sockfd after every connection has been accepted
    // notice pass by "reference" for cli_addr parameters
    // function automatically updates those parameters with the connection details
    newsockfd = accept(sockfd, (struct sockaddr*)& cli_addr, &clilen);

    if (newsockfd < 0) {
        error("Error on accept.\n");
    }

    while (1) {
        // clear the buffer
        bzero(buffer, sizeof(buffer));

        // the read function
        // must have a corresponding write() in the client code
        n = read(newsockfd, buffer, sizeof(buffer));
        if (n < 0) {
            error("Error on reading.\n");
        }
        printf("Client: %s\n", buffer); // print client's message
        bzero(buffer, sizeof(buffer)); // clear buffer

        fgets(buffer, sizeof(buffer), stdin); // get server's reply
        n = write(newsockfd, buffer, strlen(buffer)); // send the server's response to the client
        if (n < 0) {
            error("Error on writing.\n");
        }

        // break out of loop
        int i = strncmp("Bye", buffer, strlen("Bye"));
        if (i == 0) {
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}