#include "../common/Login/login_ad.h"
#include "../Functions/server_const.h"
#include "../structModels/student_struct.h"

bool admin_portal(int connectionFileDescriptor);
bool add_student(int connectionFileDescriptor);

bool admin_portal(int connectionFileDescriptor){
	if(login_admin(true, connectionFileDescriptor)){   
		//Login successful for admin
		ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
       		char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        	bzero(writeBuffer, sizeof(writeBuffer));
        	strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
		while(1){
			strcat(writeBuffer, "\n");
            		strcat(writeBuffer, ADMIN_MENU);
            		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
			if (writeBytes == -1){
                		perror("Error while writing ADMIN_MENU to client!");
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
					add_student(connectionFileDescriptor);
					break;
				case 2:
					view_student_details(connectionFileDescriptor);
					break;
				case 3:
					add_faculty(connectionFileDescriptor);
					break;
				case 4:
					view_faculty_details(connectionFileDescriptor);
					break;
				case 5: 
					activate_student(connectionFileDescriptor);
					break;
				case 6:
					block_student(connectionFileDescriptor);
					break;
				case 7:
					modify_student_details(connectionFileDescriptor);
					break;
				case 8: 
					modify_faculty_details(connectionFileDescriptor);
					break;
				case 9: 
					strcpy(writeBuffer, ADMIN_LOGOUT);
					writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
					return false;
				default: 
					strcpy(writeBuffer, ADMIN_AGAIN);
                                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
					break;
				}

		}
	}
	else{
		return false; //admin login failed
	}
	return true;
}

bool add_student(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
    	char readBuffer[1000], writeBuffer[1000];

	struct student_struct new_student, prev_student;
	//for students roll no given in FCFS format
	int studentFileDescriptor = open("STUDENT_FILE", O_RDONLY);
    	if (studentFileDescriptor == -1 && errno == ENOENT)
    	{
        	// STUDENT_FILE was never created
        	new_student.roll_no = 1;
    	}
    	else if (studentFileDescriptor == -1)
    	{
        	perror("Error while opening student file");
        	return false;
    	}
    	else
    	{
		int offset = lseek(studentFileDescriptor, -sizeof(struct student_struct), SEEK_END);
        	if (offset == -1)
        	{
            		perror("Error seeking to last Student record!");
            		return false;
        	}

        	struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct student_struct), getpid()};
        	int lockingStatus = fcntl(studentFileDescriptor, F_SETLKW, &lock);
        	if (lockingStatus == -1)
        	{
            		perror("Error obtaining read lock on Student record!");
            		return false;
       		}

        	readBytes = read(studentFileDescriptor, &prev_student, sizeof(struct student_struct));
        	if (readBytes == -1)
        	{
            		perror("Error while reading Student record from file");
            		return false;
        	}

        	lock.l_type = F_UNLCK;
        	fcntl(studentFileDescriptor, F_SETLK, &lock);

        	close(studentFileDescriptor);

        	new_student.roll_no = prev_student.roll_no + 1;
    	}
	//for students name
	writeBytes = write(connectionFileDescriptor, STUDENT_NAME, strlen(STUDENT_NAME));
    	if (writeBytes == -1)
    	{
        	perror("Error writing STUDENT_NAME message to client!");
        	return false;
    	}

    	bzero(readBuffer, sizeof(readBuffer));
    	readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
    	if (readBytes == -1)
    	{
        	perror("Error reading student name");
        	return false;
    	}

    	strcpy(new_student.name,readBuffer);
	
	//for students gender
       	bzero(writeBuffer, sizeof(writeBuffer));
	writeBytes = write(connectionFileDescriptor, STUDENT_GENDER, strlen(STUDENT_GENDER));
	if (writeBytes == -1)
        {
                perror("Error writing STUDENT_GENDER message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
	strcpy(new_student.gender,readBuffer);
	
	//for students login id
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_LOGIN, strlen(STUDENT_GENDER));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_LOGIN message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_student.login_id,readBuffer);

	//for students password
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_PASSWORD, strlen(STUDENT_GENDER));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_PASSWORD message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_student.password,readBuffer);





