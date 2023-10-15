#ifndef SEATCHECK
#define SEATCHECK

#include "../structModels/course_struct.h"
#include <fcntl.h>
bool seatsAvaliabilityCheck(char* courseIDCheck);

bool seatsAvaliabilityCheck(char* courseIDCheck){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
	
        int courseFileDescriptor = open("COURSE_FILE", O_RDONLY, 0777);
        if (courseFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
	struct flock lock;
    	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_RDLCK; // Write lock
    	lock.l_whence = SEEK_SET; // Start from the beginning of the file
    	lock.l_start = 0; // Offset 0
    	lock.l_len = 0; // Lock the entire file

    	// Try to acquire the lock in blocking mode
    	if (fcntl(courseFileDescriptor, F_SETLKW, &lock) == -1) {
        	perror("Error locking the file");
        	close(courseFileDescriptor);
        	exit(EXIT_FAILURE);
    	}
        struct course_struct course1;
	
	//checking if seats are avaliable
	while (read(courseFileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
		if(strcmp(course1.courseID, courseIDCheck)==0){
			if(course1.SeatsAvaliable==0){
				//read lock
				lock.l_type = F_UNLCK;
    				if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
        				perror("Error releasing the lock");
    				}
				close(courseFileDescriptor);
				return false;
			}
		}
	}
	//Seats avaliable
	lock.l_type = F_UNLCK;
    	if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
	close(courseFileDescriptor);
	return true;;
}



#endif 	
