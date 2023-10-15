#ifndef FACULTY_PORTAL
#define FACULTY_PORTAL

//the required headerfiles are included
#include "../Functions/server_const.h"
#include "../structModels/faculty_struct.h"
#include "../common/Login/login_fc.h"
#include "../structModels/course_struct.h"

//the list of functions in this header file
bool faculty_remove_course(int connectionFileDescriptor, char* faculty_id);
bool view_offering_course(int connectionFileDescriptor, char* faculty_id);
bool add_new_course(int connectionFileDescriptor, char* faculty_id);
bool faculty_change_password(int connectionFileDescriptor, char* faculty_id);
bool faculty_portal(int connectionFileDescriptor);


//the login and options in the faculty portal 
bool faculty_portal(int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        char tempBuffer[1000];
	
	//getting the Login ID and Password from user	
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

	//the if statement returns if ID and password matches and allowes login
	if(login_faculty(true, connectionFileDescriptor, faculty_id, pass_buffer)){
                //login successful
		bzero(readBuffer, sizeof(readBuffer));
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, F_FACULTY_LOGIN_SUCCESS);
                while(1){
                        strcat(writeBuffer, "\n");
                        strcat(writeBuffer, F_FACULTY_MENU);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        if (writeBytes == -1){
                                perror("Error while writing to client!");
                                return false;
                        }
                        bzero(writeBuffer, sizeof(writeBuffer));

                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if (readBytes == -1){
                                perror("Error while reading client's choice for ADMIN_MENU");
                                return false;
                        }
			//all the faculty options
                        int choice = atoi(readBuffer);
                        switch(choice){
                                case 1:
                                        view_offering_course(connectionFileDescriptor, faculty_id);
                                        break;
                                case 2:
                                        add_new_course(connectionFileDescriptor, faculty_id);
                                        break;
                                case 3:
                                        faculty_remove_course(connectionFileDescriptor, faculty_id);
                                        break;
                                case 4:
                                        faculty_change_password(connectionFileDescriptor, faculty_id);
                                        break;
                                case 5:
                                        strcpy(writeBuffer, F_FACULTY_LOGOUT);
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                        return false;
                                default:
                                        strcpy(writeBuffer, F_FACULTY_AGAIN);
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                        break;
                                        }

                }
        }
        else{
                return false; //faculty login failed
        }
        return true;
}

bool faculty_remove_course(int connectionFileDescriptor, char* faculty_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
	view_offering_course(connectionFileDescriptor, faculty_id);
	
	writeBytes = write(connectionFileDescriptor, COURSE_DROP, strlen(COURSE_DROP));
        if (writeBytes == -1)
        {
                perror("Error writing message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
                perror("Error reading");
                return false;
        }
	char course_id[20];
	strcpy(course_id, readBuffer);
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

        struct course_struct course1;
        int courseFileDescriptor = open("COURSE_FILE", O_RDWR, 0777);

	struct flock lock;
    	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_WRLCK; // Write lock
    	lock.l_whence = SEEK_SET; // Start from the beginning of the file
    	lock.l_start = 0; // Offset 0
    	lock.l_len = 0; // Lock the entire file

    	// Try to acquire the lock in blocking mode
    	if (fcntl(courseFileDescriptor, F_SETLKW, &lock) == -1) {
        	perror("Error locking the file");
        	close(courseFileDescriptor);
        	exit(EXIT_FAILURE);
    	}

        int flag=0;
        //searching the courses of the students enrolled
        while (read(courseFileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
                if(strcmp(course1.courseID,course_id)==0 && course1.course_flag==1 && strcmp(course1.facultyID, faculty_id)==0){
                        course1.course_flag = 0;
                        lseek(courseFileDescriptor, -1*sizeof(struct course_struct), SEEK_CUR);
                        write(courseFileDescriptor, &course1, sizeof(struct course_struct));
                        flag = 1;
                }
        }
	//Unlocking
	lock.l_type = F_UNLCK;
    	if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
        close(courseFileDescriptor);

	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	if(flag==1){
		courseFileDescriptor = open("Course_Student", O_RDWR, 0777);
		lock.l_type = F_WRLCK; // Write lock
        	lock.l_whence = SEEK_SET; // Start from the beginning of the file
        	lock.l_start = 0; // Offset 0
       	 	lock.l_len = 0; // Lock the entire file

        	// Try to acquire the lock in blocking mode
        	if (fcntl(courseFileDescriptor, F_SETLKW, &lock) == -1) {
                	perror("Error locking the file");
                	close(courseFileDescriptor);
                	exit(EXIT_FAILURE);
        	}
                struct course_student_struct course2;
                while (read(courseFileDescriptor, &course2, sizeof(struct course_student_struct)) == sizeof(struct course_student_struct)){
                        if(strcmp(course2.courseID,course_id) == 0 && course2.cs_flag==1){
                                course2.cs_flag = 0;
                                lseek(courseFileDescriptor, -sizeof(struct course_student_struct), SEEK_CUR);
                                write(courseFileDescriptor, &course2, sizeof(struct course_student_struct));
			}
		}
		//Unlocking
        	lock.l_type = F_UNLCK;
        	if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
                	perror("Error releasing the lock");
        	}
        	close(courseFileDescriptor);
    		write(connectionFileDescriptor, COURSE_DROPPED, strlen(COURSE_DROPPED));
                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                return true;
        }
        write(connectionFileDescriptor, COURSE_NOT_FOUND, strlen(COURSE_NOT_FOUND));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return true;
}



bool view_offering_course(int connectionFileDescriptor, char* faculty_id){
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
        char CourseList[1000];

	strcpy(CourseList, COURSE_LIST);
	//searching the offered course
	while (read(courseFileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
                if(strcmp(course1.facultyID, faculty_id)==0 && course1.course_flag==1){
			char tempBuffer[1000];  // Temporary buffer to construct the string
                	sprintf(tempBuffer, "Course ID: %s ", course1.courseID);
                	strcat(CourseList, tempBuffer);
                	sprintf(tempBuffer, "Course Name: %s ", course1.courseName);
                	strcat(CourseList, tempBuffer);
			sprintf(tempBuffer, "Total Seats: %s ", course1.totalSeats);
                        strcat(CourseList, tempBuffer);
			sprintf(tempBuffer, "Seats Avaliable: %d ", course1.SeatsAvaliable);
                        strcat(CourseList, tempBuffer);
                	strcat(CourseList, "\n");
              	}
	}
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
	
	//Unlocking
        lock.l_type = F_UNLCK;
        if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
            	perror("Error releasing the lock");
      	}
	close(courseFileDescriptor);
	strcpy(writeBuffer,CourseList);
        strcat(writeBuffer, EXIT);

	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing to client");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	return true;
}


bool add_new_course(int connectionFileDescriptor, char* faculty_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

        struct course_struct new_course;

        //course name
        writeBytes = write(connectionFileDescriptor, COURSE_NAME, strlen(COURSE_NAME));
        if (writeBytes == -1)
        {
                perror("Error writing NAME message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
                perror("Error reading name");
                return false;
        }
	strcpy(new_course.courseName,readBuffer);

	//for course ID
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, COURSE_ID, strlen(COURSE_ID));
        if (writeBytes == -1)
        {
                perror("Error writing  message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_course.courseID,readBuffer);

	//for faculty for that course
	strcpy(new_course.facultyID, faculty_id);
	
	//for course total seats
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, COURSE_SEATS, strlen(COURSE_SEATS));
        if (writeBytes == -1)
        {
                perror("Error writing  message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_course.totalSeats,readBuffer);

	// for avaliable seats (in this case same as total seats)
	int seatCount = atoi(readBuffer);
	new_course.SeatsAvaliable = seatCount;

	//for the flag
	new_course.course_flag = 1;

	//opening or creating a file for course
	int courseFileDescriptor = open("COURSE_FILE", O_CREAT | O_APPEND | O_WRONLY, 0777);
	if (courseFileDescriptor == -1)
        {
                perror("Error while creating / opening faculty file!");
                return false;
        }

	struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK; // Write lock
        lock.l_whence = SEEK_SET; // Start from the beginning of the file
        lock.l_start = 0; // Offset 0
        lock.l_len = 0; // Lock the entire file

        // Try to acquire the lock in blocking mode
        if (fcntl(courseFileDescriptor, F_SETLKW, &lock) == -1) {
                perror("Error locking the file");
                close(courseFileDescriptor);
                exit(EXIT_FAILURE);
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(courseFileDescriptor, &new_course, sizeof(struct course_struct));
        if (writeBytes == -1)
        {
                perror("Error while writing Faculty record to file!");
                return false;
        }
	lock.l_type = F_UNLCK;//to unlock file
    	if (fcntl(courseFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
        close(courseFileDescriptor);

	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, COURSE_ADDED, strlen(COURSE_ADDED));
        if (writeBytes == -1)
        {
                perror("Error writing message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        return true;
}



bool faculty_change_password(int connectionFileDescriptor, char* faculty_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int facultyFileDescriptor = open("FACULTY_FILE", O_RDWR, 0777);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK; // Write lock
        lock.l_whence = SEEK_SET; // Start from the beginning of the file
        lock.l_start = 0; // Offset 0
        lock.l_len = 0; // Lock the entire file

        // Try to acquire the lock in blocking mode
        if (fcntl(facultyFileDescriptor, F_SETLKW, &lock) == -1) {
                perror("Error locking the file");
                close(facultyFileDescriptor);
                exit(EXIT_FAILURE);
        }


        struct faculty_struct faculty1;
	while (read(facultyFileDescriptor, &faculty1, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
		if(strcmp(faculty1.login, faculty_id)==0){
			lseek(facultyFileDescriptor, -1 * sizeof(struct faculty_struct), SEEK_CUR);
                        //to update password
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

			strcpy(writeBuffer, F_FACULTY_CHANGE_PASSWORD);
                  
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));

			strcpy(faculty1.password, readBuffer);

			write(facultyFileDescriptor, &faculty1, sizeof(struct faculty_struct));

			lock.l_type = F_UNLCK;//to unlock file
    			if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
    			}
                        close(facultyFileDescriptor);
                        strcpy(writeBuffer, UPDATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        break;
		}
	}
	return true;
}




#endif
