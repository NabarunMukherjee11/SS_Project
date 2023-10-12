#ifndef ADMIN_PORTAL
#define ADMIN_PORTAL

#include "../common/Login/login_ad.h"
#include "../Functions/server_const.h"
#include "../structModels/faculty_struct.h"
#include "../structModels/student_struct.h"

bool admin_portal(int connectionFileDescriptor);
bool add_student(int connectionFileDescriptor);
bool add_faculty(int connectionFileDescriptor);
bool view_student_details(int connectionFileDescriptor);
bool view_faculty_details(int connectionFileDescriptor);
bool modify_student_details(int connectionFileDescriptor);
bool modify_faculty_details(int connectionFileDescriptor);
bool activate_student(int connectionFileDescriptor);
bool block_student(int connectionFileDescriptor);


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
					break; /*
				case 7:
					modify_student_details(connectionFileDescriptor);
					break;
				case 8: 
					modify_faculty_details(connectionFileDescriptor);
					break;*/
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
	
	//for students age
       	bzero(writeBuffer, sizeof(writeBuffer));
	writeBytes = write(connectionFileDescriptor, STUDENT_AGE, strlen(STUDENT_AGE));
	if (writeBytes == -1)
        {
                perror("Error writing STUDENT_AGE message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
	strcpy(new_student.age,readBuffer);

	//for students age
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_ADDRESS, strlen(STUDENT_ADDRESS));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_ADDRESS message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_student.address,readBuffer);
	
	//for students email
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_EMAIL, strlen(STUDENT_EMAIL));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_EMAIL message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
	const char *src = "@iiitb.ac.in";
        strcat(readBuffer, src);
        strcpy(new_student.email,readBuffer);

	//for students login
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_LOGIN, strlen(STUDENT_EMAIL));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_LOGIN message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_student.login,readBuffer);

	//for students password
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, STUDENT_PASSWORD, strlen(STUDENT_PASSWORD));
        if (writeBytes == -1)
        {
                perror("Error writing STUDENT_PASSWORD message to client!");
                return false;
        }

	bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_student.password,readBuffer);

	new_student.active = true;

	studentFileDescriptor = open("STUDENT_FILE", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    	if (studentFileDescriptor == -1)
    	{
        	perror("Error while creating / opening student file!");
        	return false;
    	}
	bzero(writeBuffer, sizeof(writeBuffer));
    	writeBytes = write(studentFileDescriptor, &new_student, sizeof(struct student_struct));
    	if (writeBytes == -1)
   	{
        	perror("Error while writing Student record to file!");
        	return false;
    	}

    	close(studentFileDescriptor);

	bzero(writeBuffer, sizeof(writeBuffer));
	writeBytes = write(connectionFileDescriptor, STUDENT_ADDED, strlen(STUDENT_ADDED));
        if (writeBytes == -1)
        {
                perror("Error writing message");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
	return true;
}


bool add_faculty(int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

        struct faculty_struct new_faculty;

	//faculty name
	writeBytes = write(connectionFileDescriptor, FACULTY_NAME, strlen(FACULTY_NAME));
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

        strcpy(new_faculty.name,readBuffer);

        //for faculty department
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_DEPARTMENT, strlen(FACULTY_DEPARTMENT));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_DEPARTMENT message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_faculty.department,readBuffer);

	// for faculty designation
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_DESIGNATION, strlen(FACULTY_DESIGNATION));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_DESIGNATION message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_faculty.designation,readBuffer);

        //for faculty email id
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_EMAIL, strlen(FACULTY_EMAIL));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_EMAIL message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
	const char *src = "@iiitb.ac.in";
        strcat(readBuffer, src);
        strcpy(new_faculty.email,readBuffer);

	// for faculty address
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_ADDRESS, strlen(FACULTY_ADDRESS));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_ADDRESS message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_faculty.address,readBuffer);


	// for faculty login id
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_LOGIN, strlen(FACULTY_LOGIN));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_LOGIN message to client!");
                return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_faculty.login,readBuffer);

	// for faculty password
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_PASSWORD, strlen(FACULTY_PASSWORD));
        if (writeBytes == -1)
        {
                perror("Error writing FACULTY_PASSWORD message to client!");
                return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        strcpy(new_faculty.password,readBuffer);


        int facultyFileDescriptor = open("FACULTY_FILE", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while creating / opening faculty file!");
                return false;
        }
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(facultyFileDescriptor, &new_faculty, sizeof(struct faculty_struct));
        if (writeBytes == -1)
        {
                perror("Error while writing Faculty record to file!");
                return false;
        }

        close(facultyFileDescriptor);

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connectionFileDescriptor, FACULTY_ADDED, strlen(FACULTY_ADDED));
        if (writeBytes == -1)
        {
                perror("Error writing message to client!");
                return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, &readBuffer, sizeof(readBuffer));
        return true;
}

bool view_student_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
	int studentFileDescriptor = open("STUDENT_FILE", O_RDONLY);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
	struct student_struct student1, student;
	char StudentList[1000];
	StudentList[0]='\0';
        while (read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
		char tempBuffer[1000];  // Temporary buffer to construct the string
    		sprintf(tempBuffer, "Login: %s ", student1.login);
		strcat(StudentList, tempBuffer);
		sprintf(tempBuffer, "Name: %s ", student1.name);
		strcat(StudentList, tempBuffer);
		strcat(StudentList, "\n");
	}
	lseek(studentFileDescriptor, 0, SEEK_SET);

	strcpy(writeBuffer,StudentList);
	strcat(writeBuffer, STUDENT_DETAILS);

	bzero(StudentList, sizeof(StudentList));
	StudentList[0]='\0';

	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
	if (writeBytes == -1)
        {
                perror("Error while writing to file!");
                return false;
        }
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	char login[20];
	
	strcpy(login, readBuffer);
	
	bzero(writeBuffer, sizeof(writeBuffer));
	bzero(readBuffer, sizeof(readBuffer));
	
	while(read(studentFileDescriptor, &student, sizeof(struct student_struct)) == sizeof(struct student_struct)){
		if (strcmp(student.login, login) == 0){
			char myStr[100];
			sprintf(myStr, "Roll No: %d\n", student.roll_no);
			strcat(StudentList, myStr);

			sprintf(myStr, "Name: %s\n", student.name);
			strcat(StudentList, myStr);

			sprintf(myStr, "Email: %s\n", student.email);
			strcat(StudentList, myStr);

			sprintf(myStr, "Address: %s\n", student.address);
			strcat(StudentList, myStr);

			sprintf(myStr, "Age: %s\n", student.age);
			strcat(StudentList, myStr);

			sprintf(myStr, "Login ID: %s\n", student.login);
			strcat(StudentList, myStr);

			strcpy(writeBuffer, StudentList);
			strcat(writeBuffer, "Press any key to exit\n");
			writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
       		 	if (writeBytes == -1)
        		{
          		      	perror("Error while writing to file!");
                		return false;
        		}
			readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
			return true;
		}
	}
	strcpy(writeBuffer,"Wrong Option!\n Press any key to exit\n");
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        return false;
}

bool view_faculty_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int facultyFileDescriptor = open("FACULTY_FILE", O_RDONLY);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }
        struct faculty_struct faculty1, faculty;
        char FacultyList[1000];
        FacultyList[0]='\0';
        while (read(facultyFileDescriptor, &faculty1, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
                char tempBuffer[1000];  // Temporary buffer to construct the string
                sprintf(tempBuffer, "Login: %s ", faculty1.login);
                strcat(FacultyList, tempBuffer);
                sprintf(tempBuffer, "Name: %s ", faculty1.name);
                strcat(FacultyList, tempBuffer);
                strcat(FacultyList, "\n");
        }
        lseek(facultyFileDescriptor, 0, SEEK_SET);

	strcpy(writeBuffer,FacultyList);
        strcat(writeBuffer, STUDENT_DETAILS);

        bzero(FacultyList, sizeof(FacultyList));
        FacultyList[0]='\0';

        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing to file!");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        char login[20];

        strcpy(login, readBuffer);

        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

        while(read(facultyFileDescriptor, &faculty, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
                if (strcmp(faculty.login, login) == 0){
			char myStr[1000];
                        sprintf(myStr, "Name: %s\n", faculty.name);
                        strcat(FacultyList, myStr);

                        sprintf(myStr, "Department: %s\n", faculty.department);
                        strcat(FacultyList, myStr);

                        sprintf(myStr, "Email: %s\n", faculty.email);
                        strcat(FacultyList, myStr);

                        sprintf(myStr, "Address: %s\n", faculty.address);
                        strcat(FacultyList, myStr);

                        sprintf(myStr, "Login ID: %s\n", faculty.login);
                        strcat(FacultyList, myStr);

                        strcpy(writeBuffer, FacultyList);
                        strcat(writeBuffer, "Press any key to exit\n");
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        if (writeBytes == -1)
                        {
                                perror("Error while writing to file!");
                                return false;
                        }
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        return true;
                }
        }
        strcpy(writeBuffer,"Wrong Option!\n Press any key to exit\n");
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        return false;
}

bool block_student(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int studentFileDescriptor = open("STUDENT_FILE", O_RDONLY);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
        struct student_struct student1;
        char StudentList[1000];
        StudentList[0]='\0';
        while (read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if(student1.active == false){
			continue;
		}
		char tempBuffer[1000];  // Temporary buffer to construct the string
                sprintf(tempBuffer, "Login: %s ", student1.login);
                strcat(StudentList, tempBuffer);
                sprintf(tempBuffer, "Name: %s ", student1.name);
                strcat(StudentList, tempBuffer);
                strcat(StudentList, "\n");
        }
        lseek(studentFileDescriptor, 0, SEEK_SET);

        strcpy(writeBuffer,StudentList);
        strcat(writeBuffer, STUDENT_BLOCK);

        bzero(StudentList, sizeof(StudentList));
        StudentList[0]='\0';

        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));

	if (writeBytes == -1)
        {
                perror("Error while writing to file!");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        char login[20];

        strcpy(login, readBuffer);

        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

	while(read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student1.login, login) == 0){
			student1.active = false;
			strcpy(writeBuffer, STUDENT_BLOCKED);
			writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
			readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
			return true;
		}
	}
	strcpy(writeBuffer, STUDENT_NOT_BLOCKED);
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	return false;
}


bool activate_student(int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int studentFileDescriptor = open("STUDENT_FILE", O_RDONLY);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
        struct student_struct student1;
        char StudentList[1000];
        StudentList[0]='\0';
        while (read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if(student1.active == true){
			continue;
		}
		char tempBuffer[1000];  // Temporary buffer to construct the string
                sprintf(tempBuffer, "Login: %s ", student1.login);
                strcat(StudentList, tempBuffer);
                sprintf(tempBuffer, "Name: %s ", student1.name);
                strcat(StudentList, tempBuffer);
                strcat(StudentList, "\n");
        }
        lseek(studentFileDescriptor, 0, SEEK_SET);

        strcpy(writeBuffer,StudentList);
        strcat(writeBuffer, STUDENT_ACTIVE);

        bzero(StudentList, sizeof(StudentList));
        StudentList[0]='\0';

        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));

        if (writeBytes == -1)
        {
                perror("Error while writing to file!");
                return false;
        }
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        char login[20];

        strcpy(login, readBuffer);

        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));

        while(read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student1.login, login) == 0){
                        student1.active = true;
			strcpy(writeBuffer, STUDENT_ACTIVATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        return true;
                }
        }
        strcpy(writeBuffer, STUDENT_NOT_ACTIVATED);
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return false;
}




#endif


