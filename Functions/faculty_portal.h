#ifndef FACULTY_PORTAL
#define FACULTY_PORTAL

#include "../Functions/server_const.h"
#include "../structModels/faculty_struct.h"
#include "../common/Login/login_fc.h"

bool faculty_portal(int connectionFileDescriptor);

bool faculty_portal(int connectionFileDescriptor){
        if(login_faculty(true, connectionFileDescriptor)){
                ssize_t readBytes, writeBytes;
                char readBuffer[1000], writeBuffer[1000];
                char tempBuffer[1000];
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
                              /*  case 1:
                                        view_offering_course(connectionFileDescriptor);
                                        break;
                                case 2:
                                        add_new_course(connectionFileDescriptor);
                                        break;
                                case 3:
                                        remove_course(connectionFileDescriptor);
                                        break;
                                case 4:
                                        update_course(connectionFileDescriptor);
                                        break;
                                case 5:
                                        change_password(connectionFileDescriptor);
                                        break;*/
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
#endif
