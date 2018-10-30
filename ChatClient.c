#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAXDATASIZE 500 // max number of bytes we can get at once 
#define HANDLESIZE 11   // max length for handle name
#define SYMBOLSIZE 3   // max length for handle name

int connect(char*, char*); // Connect to server

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;  
    char symbol[SYMBOLSIZE] = "> ";
    char handle[HANDLESIZE];
    char message[MAXDATASIZE];
    char* host = argv[1];
    char* port = argv[2]; 

    memset(handle, '\0', HANDLESIZE);

    // Ensure that a host and port number have been passed
    if (argc != 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // First get clt's handle name
    printf("Enter your handle name (Max 10 Characters): ");
    fflush(stdout);

    fgets(handle, HANDLESIZE, stdin);

    int handleLength = strlen(handle) - 1; 

    // Ensure handle was created
    while(handleLength == 0) {
        printf("Handle name must exist.\nEnter your handle name (Max 10 Characters): ");
        fflush(stdout);

        fgets(handle, HANDLESIZE, stdin);

        handleLength = strlen(handle) - 1;
    }

    // Remove trailing newline from handle
    strtok(handle, "\n");

    // Connect to server
    sockfd = connect(host, port);

    // Validate turn
    int waiting = 0; 

    // Continues connection until client enters '\quit'
    while (1) {
        if (!waiting) {
            printf("%s%s ", handle, symbol);
            
            memset(message, '\0', MAXDATASIZE);
            fgets(message, MAXDATASIZE, stdin);

            // Check to see if client wants to exit
            const char closeClient[10] = "\\quit";
            char* exitClient = NULL;
            exitClient = strstr(message, closeClient);

            if(exitClient != NULL) {
                printf("Exiting Chat!\n");
                
                close(sockfd);
                break;
            }
            char* completeMessage = (char*)malloc(HANDLESIZE + SYMBOLSIZE + MAXDATASIZE);
            memset(completeMessage, '\0', HANDLESIZE + SYMBOLSIZE + MAXDATASIZE);

            strcat(completeMessage, handle);
            strcat(completeMessage, symbol);
            strcat(completeMessage, message);

            n = write(sockfd, completeMessage, strlen(completeMessage));

            if (n < 0) 
                error("ERROR writing to socket");

            waiting = 1;
        }
        else {
            bzero(message, MAXDATASIZE + HANDLESIZE);
            n = read(sockfd, message, MAXDATASIZE + HANDLESIZE);

            if (n < 0) 
                error("ERROR reading from socket");

            printf("%s", message);

            waiting = 0;
        }
    }

    close(sockfd);
    return 0;
}

// Connects to server
int connect(char* host, char* port){
    int sockfd, connection;  
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((connection = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(connection));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        freeaddrinfo(servinfo);
        return 2;
    }

    freeaddrinfo(servinfo);

    return sockfd;
}