#ifndef FACULTY_PORTAL
#define FACULTY_PORTAL

#include "../Functions/server_const.h"
#include "../structModels/faculty_struct.h"
#include "../common/Login/login_fc.h"
#include "../structModels/course_struct.h"


bool view_offering_course(int connectionFileDescriptor, char* faculty_id);
bool add_new_course(int connectionFileDescriptor, char* faculty_id);
bool faculty_change_password(int connectionFileDescriptor, char* faculty_id);
bool faculty_portal(int connectionFileDescriptor);

bool faculty_portal(int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        char tempBuffer[1000];
		
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


	if(login_faculty(true, connectionFileDescriptor, faculty_id, pass_buffer)){
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
                        int choice = atoi(readBuffer);
                        switch(choice){
                                case 1:
                                        view_offering_course(connectionFileDescriptor, faculty_id);
                                        break;
                                case 2:
                                        add_new_course(connectionFileDescriptor, faculty_id);
                                        break;/*
                                case 3:
                                        remove_course(connectionFileDescriptor);
                                        break;
                                case 4:
                                        update_course(connectionFileDescriptor);
                                        break;*/
                                case 5:
                                        faculty_change_password(connectionFileDescriptor, faculty_id);
                                        break;
                                case 6:
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


bool view_offering_course(int connectionFileDescriptor, char* faculty_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

	int courseFileDescriptor = open("COURSE_FILE", O_RDONLY, 0777);
        if (courseFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
        struct course_struct course1;
        char CourseList[1000];
        CourseList[0]='\0';
	strcpy(CourseList, COURSE_LIST);
	while (read(courseFileDescriptor, &course1, sizeof(struct course_struct)) == sizeof(struct course_struct)){
                if(strcmp(course1.facultyID, faculty_id)==0){
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

	//opening or creating a file for course
	int courseFileDescriptor = open("COURSE_FILE", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
	if (courseFileDescriptor == -1)
        {
                perror("Error while creating / opening faculty file!");
                return false;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(courseFileDescriptor, &new_course, sizeof(struct course_struct));
        if (writeBytes == -1)
        {
                perror("Error while writing Faculty record to file!");
                return false;
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
