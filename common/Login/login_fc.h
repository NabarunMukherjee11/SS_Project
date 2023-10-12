#ifndef LOGIN_FACULTY
#define LOGIN_FACULTY

#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`
#include "../../Functions/server_const.h"
#include "../password_check.h"

bool login_faculty(bool isAdmin, int connectionFileDescriptor);

bool login_faculty(bool isAdminn, int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        char tempBuffer[1000];
        bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, F_FACULTY_LOGIN_WELCOME);
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        if(writeBytes==-1){
                perror("Error in writing\n");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	if(readBytes==-1){
                perror("Error in writing\n");
                return false;
        }
	char pass_buffer[20];
	char faculty_id[20];
	
	strcpy(faculty_id, readBuffer);
	printf("%s\n",readBuffer);
	bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));

	strcpy(writeBuffer, F_FACULTY_PASSWORD);
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
	if(writeBytes==-1){
                perror("Error in writing\n");
                return false;
        }
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	if(readBytes==-1){
		perror("Error in writing\n");
                return false;
        }
        bzero(writeBuffer, sizeof(writeBuffer));

	strcpy(pass_buffer, readBuffer);

	if(faculty_password_checker(faculty_id, pass_buffer)){
		return true;
	}
	return false;
}


#endif





