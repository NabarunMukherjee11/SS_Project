#include <stdio.h> // Import for `printf` & `perror` functions
#include <errno.h> // Import for `errno` variable

#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture

#include <string.h>  // Import for string functions
#include <stdbool.h> // Import for `bool` data type
#include <stdlib.h>  // Import for `atoi` funtions

#include "../Functions/student_portal.h"
#include "../Functions/admin_portal.h"
#include "../Functions/server_const.h"
#include "../Functions/faculty_portal.h"

void portal_handler(int connectionFileDescriptor){
	printf("Connection is made\n");
	char readBuffer[1000], writeBuffer[1000];
    	ssize_t readBytes, writeBytes;
	int choice;
	writeBytes = write(connectionFileDescriptor, INITIAL_PROMPT, strlen(INITIAL_PROMPT));
	if (writeBytes == -1){
        	perror("Error while sending data to the user");
	}
	else{
		bzero(readBuffer, sizeof(readBuffer));
		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	       	if(readBytes == -1){
			perror("Error while reading from client");
		}
		else if(readBytes == 0){
			printf("No data was sent to the server\n");
		}
		else{
			choice = atoi(readBuffer);
			switch(choice){
				case 1:
					admin_portal(connectionFileDescriptor);
					break;
				case 2:
					faculty_portal(connectionFileDescriptor);
					break;
				case 3: 
					student_portal(connectionFileDescriptor);
					break; 
				default:
					break;
				}
		}
	}
	printf("Closing the connection to server\n");
}

void main()
{
    int socketFileDescriptor, socketBindStatus, socketListenStatus, connectionFileDescriptor;
    struct sockaddr_in serverAddress, clientAddress;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(8080);              // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    socketBindStatus = bind(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (socketBindStatus == -1)
    {
        perror("Error while binding to server socket!");
        exit(0);
    }

    socketListenStatus = listen(socketFileDescriptor, 20);
    if (socketListenStatus == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socketFileDescriptor);
        exit(0);
    }

    int clientSize;
    while (1)
    {
        clientSize = (int)sizeof(clientAddress);
        connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&clientAddress, &clientSize);
        if (connectionFileDescriptor == -1){
		perror("Error while connecting to client!");
        	close(socketFileDescriptor);
		exit(1);
        }
	else{
		if(!fork()){ // child process will handle this client socket
			portal_handler(connectionFileDescriptor);
			close(connectionFileDescriptor);
			exit(0);
		}
		
	}
    }
    close(socketFileDescriptor);
    exit(0);
}










