#ifndef STUDENT_PORTAL
#define STUDENT_PORTAL

#include "../Functions/server_const.h"
#include "../structModels/student_struct.h"
#include "../common/Login/login_st.h"
#include "../common/seatsAvaliabilityCheck.h"

//Functions inside this header file are listed here
bool remove_course(int connectionFileDescriptor, char* student_id);
bool view_enrolled_course(int connectionFileDescriptor, char* student_id);
bool new_course(int connectionFileDescriptor, char *student_id);
bool view_all_course(int connectionFileDescriptor, char* student_id);
bool change_password(int connectionFileDescriptor, char* student_id);
bool student_portal(int connectionFileDescriptor);

bool student_portal(int connectionFileDescriptor){
	//creating read and write buffer
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        char tempBuffer[1000];
	
	//printing the LOGIN PAGE and taking the STUDENT ID
	strcpy(writeBuffer, S_STUDENT_LOGIN_WELCOME);
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

	//copying the ID and Password into two buffers
        char pass_buffer[20];
        char student_id[20];
	strcpy(student_id, readBuffer);
	bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
	strcpy(writeBuffer, S_STUDENT_PASSWORD);
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

	//checking if the password is correct and student is not blocked using this function whose path is included in the top
        if(login_student(true, connectionFileDescriptor, student_id, pass_buffer)){
                //successful login
		bzero(readBuffer, sizeof(readBuffer));
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, S_STUDENT_LOGIN_SUCCESS);
		while(1){
                        strcat(writeBuffer, "\n");
                        strcat(writeBuffer, S_STUDENT_MENU);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        if (writeBytes == -1){
                                perror("Error while writing to client!");
                                return false;
                        }
                        bzero(writeBuffer, sizeof(writeBuffer));

			//Taking the choice from the user for  the options in STUDENT MENU
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if (readBytes == -1){
                                perror("Error while reading client's choice for ADMIN_MENU");
                                return false;
                        }
                        int choice = atoi(readBuffer);
                        switch(choice){
                                case 1:
                                        view_all_course(connectionFileDescriptor, student_id);
                                        break; 
                                case 2:
                                        new_course(connectionFileDescriptor, student_id);
                                        break;
                                case 3:
                                        remove_course(connectionFileDescriptor, student_id);
                                        break;
                                case 4:
                                        view_enrolled_course(connectionFileDescriptor, student_id);
                                        break;
				case 5:
                                        change_password(connectionFileDescriptor, student_id);
                                        break;
                                case 6:
                                        strcpy(writeBuffer, S_STUDENT_LOGOUT);//to logout
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                        return false;
                                default:
                                        strcpy(writeBuffer, S_STUDENT_AGAIN);//for invalid option
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                        break;
                                        }

                }
        }
        else{
		return false; //student login failed
        }
        return true; 
}

bool remove_course(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	view_enrolled_course(connectionFileDescriptor, student_id);
	
	writeBytes = write(connectionFileDescriptor, COURSE_DROP, strlen(COURSE_DROP));
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	char course_id[20];
	strcpy(course_id, readBuffer);
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	struct course_student_struct course1;
	int courseFileDescriptor = open("Course_Student", O_RDWR, 0777);

        int flag=0;
	//searching the courses of the students enrolled
        while (read(courseFileDescriptor, &course1, sizeof(struct course_student_struct)) == sizeof(struct course_student_struct)){
                if(strcmp(course1.studentID,student_id)==0 && course1.cs_flag==1 && strcmp(course1.courseID,course_id)==0){
			course1.cs_flag = 0;
			lseek(courseFileDescriptor, -sizeof(struct course_student_struct), SEEK_CUR);
			write(courseFileDescriptor, &course1, sizeof(struct course_student_struct));
			flag = 1;
		}
	}
	close(courseFileDescriptor);

	//only if we made changes in student course we will make changes in Seats Avaliable
	if(flag==1){
		courseFileDescriptor = open("COURSE_FILE", O_RDWR, 0777);
		struct course_struct course2;
		while (read(courseFileDescriptor, &course2, sizeof(struct course_struct)) == sizeof(struct course_struct)){
			if(strcmp(course2.courseID,course_id) == 0){
				course2.SeatsAvaliable++;
				lseek(courseFileDescriptor, -sizeof(struct course_struct), SEEK_CUR);
                        	write(courseFileDescriptor, &course2, sizeof(struct course_struct));
				close(courseFileDescriptor);
				write(connectionFileDescriptor, COURSE_DROPPED, strlen(COURSE_DROPPED));
				readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
				return true;
			}
		}
	}
	write(connectionFileDescriptor, COURSE_NOT_FOUND, strlen(COURSE_NOT_FOUND));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return true;
}


bool view_enrolled_course(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

	//opening the file which has course-student relation in read only mode
	int courseFileDescriptor = open("Course_Student", O_RDONLY, 0777);
        if (courseFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
        struct course_student_struct course1;
        char CourseList[1000];
        CourseList[0]='\0';
        strcpy(CourseList, COURSE_ENROLL);
	
	//searching all the courses and the students enrolled
        while (read(courseFileDescriptor, &course1, sizeof(struct course_student_struct)) == sizeof(struct course_student_struct)){
		if(strcmp(course1.studentID,student_id)==0 && course1.cs_flag==1){               
			//if the student ID matches and the course is not blocked we enter here
			char tempBuffer[1000];  // Temporary buffer to construct the string
                        sprintf(tempBuffer, "Course ID: %s ", course1.courseID);
                        strcat(CourseList, tempBuffer);
                        strcat(CourseList, "\n");
		}

        }
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	close(courseFileDescriptor);
        strcpy(writeBuffer, CourseList);
        strcat(writeBuffer, EXIT);

	//writing the data to the Client 
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing to file!");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return true;
}

	
bool new_course(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
		
	//calling this function to view all courses
	view_all_course(connectionFileDescriptor, student_id);

	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	//reading the course ID
	writeBytes = write(connectionFileDescriptor, ENROLL, strlen(ENROLL));
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	
	//copying the course ID to a buffer
	char courseIDCheck[20];
	strcpy(courseIDCheck, readBuffer);

	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	//this function will check if the course has avaliable seats
	if(seatsAvaliabilityCheck(courseIDCheck)== false){
		 printf("Rejected\n"); 
		 writeBytes = write(connectionFileDescriptor, NO_SEAT, strlen(NO_SEAT));
		 readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
		 return true;
	}
	//seats avaliable
	int fileDescriptor = open("Course_Student", O_CREAT | O_RDWR, 0777);

	if(fileDescriptor == -1){
		perror("Error opening file");
		return false;
	}
	
	//checking if the student is already enrolled
	struct course_student_struct course_student;
	while (read(fileDescriptor, &course_student, sizeof(struct course_student_struct)) == sizeof(struct course_student_struct)){
		printf("after FD while\n");
		if(strcmp(course_student.courseID, courseIDCheck)==0 && strcmp(course_student.studentID, student_id)==0){
			writeBytes = write(connectionFileDescriptor, ALREADY_ENROLLED, strlen(ALREADY_ENROLLED));
			readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
			close(fileDescriptor);
			return true;
		}
	}
	//not already enrolled
	close(fileDescriptor);
	//enrolling the student
	fileDescriptor = open("Course_Student", O_WRONLY | O_APPEND , 0777);
	strcpy(course_student.courseID, courseIDCheck);
       	strcpy(course_student.studentID, student_id);
	
	//set the flag to know the student is taking this course
	course_student.cs_flag=1;

	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
	
	write(fileDescriptor, &course_student, sizeof(struct course_student_struct));
	writeBytes = write(connectionFileDescriptor, ENROLLED, strlen(ENROLLED));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	close(fileDescriptor);
	
	fileDescriptor = open("COURSE_FILE", O_RDWR, 0777);

	struct course_struct course1;

	//decreasing the seat count in the course
        while (read(fileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
                if(strcmp(course1.courseID, courseIDCheck)==0){
                      	course1.SeatsAvaliable--;
                      	lseek(fileDescriptor, -1*sizeof(struct course_struct), SEEK_CUR);
                  	write(fileDescriptor, &course1, sizeof(struct course_struct));
                   	close(fileDescriptor);
                }
        }
	return true;
}

	
bool view_all_course(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

        int courseFileDescriptor = open("COURSE_FILE", O_RDONLY, 0777);
        if (courseFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
	//setting readlock
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
        CourseList[0]='\0';
        strcpy(CourseList, COURSE_LIST);
	//traversing and getting details of all the courses
        while (read(courseFileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
             	if(course1.course_flag==1){           
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

	//unlocking
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
                perror("Error while writing to file!");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return true;
}

bool change_password(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int studentFileDescriptor = open("STUDENT_FILE", O_RDWR, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
	 //setting writelock
        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK; // Write lock
        lock.l_whence = SEEK_SET; // Start from the beginning of the file
        lock.l_start = 0; // Offset 0
        lock.l_len = 0; // Lock the entire file

        // Try to acquire the lock in blocking mode
        if (fcntl(studentFileDescriptor, F_SETLKW, &lock) == -1) {
                perror("Error locking the file");
                close(studentFileDescriptor);
                exit(EXIT_FAILURE);
        }

        struct student_struct student1;
        while (read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if(strcmp(student1.login, student_id)==0){
                        lseek(studentFileDescriptor, -1 * sizeof(struct student_struct), SEEK_CUR);
                        //to update password
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

                        strcpy(writeBuffer, F_FACULTY_CHANGE_PASSWORD);

                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));

                        strcpy(student1.password, readBuffer);

                        write(studentFileDescriptor, &student1, sizeof(struct student_struct));
                        //unlocking
			lock.l_type = F_UNLCK;
    			if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
    			}
			close(studentFileDescriptor);
                        strcpy(writeBuffer, UPDATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        break;
                }
        }
	return true;
}

#endif
