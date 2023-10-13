#ifndef STUDENT_PORTAL
#define STUDENT_PORTAL

#include "../Functions/server_const.h"
#include "../structModels/student_struct.h"
#include "../common/Login/login_st.h"

bool change_password(int connectionFileDescriptor, char* student_id);
bool student_portal(int connectionFileDescriptor);

bool student_portal(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        char tempBuffer[1000];
	
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
//stop

        if(login_student(true, connectionFileDescriptor, student_id, pass_buffer)){
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

                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if (readBytes == -1){
                                perror("Error while reading client's choice for ADMIN_MENU");
                                return false;
                        }
                        int choice = atoi(readBuffer);
                        switch(choice){
                              /*  case 1:
                                        view_all_course(connectionFileDescriptor);
                                        break;
                                case 2:
                                        add_new_course(connectionFileDescriptor);
                                        break;
                                case 3:
                                        remove_course(connectionFileDescriptor);
                                        break;
                                case 4:
                                        view_enrolled_course(connectionFileDescriptor);
                                        break;*/
				case 5:
                                        change_password(connectionFileDescriptor, student_id);
                                        break;
                                case 6:
                                        strcpy(writeBuffer, S_STUDENT_LOGOUT);
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                        return false;
                                default:
                                        strcpy(writeBuffer, S_STUDENT_AGAIN);
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

bool change_password(int connectionFileDescriptor, char* student_id){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int studentFileDescriptor = open("STUDENT_FILE", O_RDWR, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
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
