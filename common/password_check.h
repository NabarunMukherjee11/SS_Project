#ifndef PASSWORD_CHECKER
#define PASSWORD_CHECKER

#include "../structModels/student_struct.h"
#include "../structModels/faculty_struct.h"
#include <fcntl.h>

bool faculty_password_checker(char *login_id, char *password);
bool student_password_checker(char *login_id, char *password);

//to check password of faculty
bool faculty_password_checker(char *login_id, char *password){
        int fileDescriptor = open("FACULTY_FILE", O_RDONLY);

        if (fileDescriptor == -1) {
                perror("Error opening file");
                return 1;
        }
	struct flock lock;
    	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_RDLCK; // Read lock
   	lock.l_whence = SEEK_SET; // Start from the beginning of the file
    	lock.l_start = 0; // Offset 0
    	lock.l_len = 0; // Lock the entire file

    	// Try to acquire the lock in blocking mode
    	if (fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        	perror("Error locking the file");
        	close(fileDescriptor);
        	exit(EXIT_FAILURE);
   	}
        struct faculty_struct faculty;

        while (read(fileDescriptor, &faculty, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
                if (strcmp(faculty.login, login_id) == 0) {
                        if(strcmp(faculty.password, password) == 0){
                                printf("Password match\n");
				//unlocking
				lock.l_type = F_UNLCK;
    				if (fcntl(fileDescriptor, F_SETLK, &lock) == -1) {
        				perror("Error releasing the lock");
    				}
				close(fileDescriptor);
				return true;
                        }
                }
        }
	//unlocking
      	lock.l_type = F_UNLCK;
        if (fcntl(fileDescriptor, F_SETLK, &lock) == -1) {
       		perror("Error releasing the lock");
   	}
   	close(fileDescriptor);
        return false;
}

bool student_password_checker(char *login_id, char *password){
        int fileDescriptor = open("STUDENT_FILE", O_RDONLY);

        if(fileDescriptor == -1) {
                perror("Error opening file");
                return false;
        }
	struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK; // Read lock
        lock.l_whence = SEEK_SET; // Start from the beginning of the file
        lock.l_start = 0; // Offset 0
        lock.l_len = 0; // Lock the entire file

        // Try to acquire the lock in blocking mode
        if (fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
                perror("Error locking the file");
                close(fileDescriptor);
                exit(EXIT_FAILURE);
        }
        struct student_struct student;

        while (read(fileDescriptor, &student, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student.login, login_id) == 0) {
			if(student.active == false){
				//unlocking
                                lock.l_type = F_UNLCK;
                                if (fcntl(fileDescriptor, F_SETLK, &lock) == -1) {
                                        perror("Error releasing the lock");
                                }
                                close(fileDescriptor);
				return false;
			}
			else if(strcmp(student.password, password) == 0){
                                //unlocking
                                lock.l_type = F_UNLCK;
                                if (fcntl(fileDescriptor, F_SETLK, &lock) == -1) {
                                        perror("Error releasing the lock");
                                }
                                close(fileDescriptor);
				printf("Password match\n");
                                return true;
                        }
                }
        }
	//unlocking
   	lock.l_type = F_UNLCK;
   	if (fcntl(fileDescriptor, F_SETLK, &lock) == -1) {
              	perror("Error releasing the lock");
   	}
     	close(fileDescriptor);
        return false;
}


#endif
