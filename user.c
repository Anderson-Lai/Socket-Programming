#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h> // to store information about a host
#include <strings.h>

#define nullptr NULL

void error (const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main (int argc, char** argv) {
    
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server; // short for host entry (an entry in a database)
    
    char buffer[255];
    if (argc < 3) {
        fprintf(stderr, "usage: %s, hostname, port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error creating socket.\n");
    }

    // gets the name, returns a hostent* containg the ip address(es)
    // obtained from the host name
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host.\n");
    }

    // remove all info just in case
    bzero((char*)& serv_addr, sizeof(serv_addr));

    // set the family for serv_addr
    serv_addr.sin_family = AF_INET;

    // set the ip address by copying it from gethostbyname()
    bcopy((char*)& server->h_addr_list[0], (char*)& serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);
    
    
    return 0;
}