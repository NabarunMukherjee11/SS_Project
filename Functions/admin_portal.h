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
	close(studentFileDescriptor);
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

	//for students address
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
        writeBytes = write(connectionFileDescriptor, STUDENT_LOGIN, strlen(STUDENT_LOGIN));
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
	
	//make student active from the beginning
	new_student.active = true;
	

	studentFileDescriptor = open("STUDENT_FILE", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    	if (studentFileDescriptor == -1)
    	{
        	perror("Error while creating / opening student file!");
        	return false;
    	}
	struct flock lock;
    	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_WRLCK; // Write lock
    	lock.l_whence = SEEK_SET; // Start from the beginning of the file
    	lock.l_start = 0; // Offset 0
    	lock.l_len = 0; // Lock the entire file

    	// Try to acquire the lock in blocking mode
    	if (fcntl(studentFileDescriptor, F_SETLKW, &lock) == -1){
        	perror("Error locking the file");
        	close(studentFileDescriptor);
        	exit(EXIT_FAILURE);
    	}
	//writing the students data into the file
	bzero(writeBuffer, sizeof(writeBuffer));
    	writeBytes = write(studentFileDescriptor, &new_student, sizeof(struct student_struct));
    	if (writeBytes == -1)
   	{
        	perror("Error while writing Student record to file!");
        	return false;
    	}
	//releasing the lock
	lock.l_type = F_UNLCK;
    	if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
    	close(studentFileDescriptor);
	
	//writing a message for add confirmation
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

	//opening the file to add data
        int facultyFileDescriptor = open("FACULTY_FILE", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while creating / opening faculty file!");
                return false;
        }
	//setting the write lock
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
	bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(facultyFileDescriptor, &new_faculty, sizeof(struct faculty_struct));
        if (writeBytes == -1)
        {
                perror("Error while writing Faculty record to file!");
                return false;
        }
	//releasing the lock
	lock.l_type = F_UNLCK;
  	if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}

        close(facultyFileDescriptor);

	//writing a confirmation message
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


//to view the added student
bool view_student_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];

	//the STUDENT_FILE has the details of the student
	int studentFileDescriptor = open("STUDENT_FILE", O_RDONLY, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
	struct flock lock;
	memset(&lock, 0, sizeof(lock));
    	lock.l_type = F_RDLCK; // Read lock
    	lock.l_whence = SEEK_SET; // Start from the beginning of the file
    	lock.l_start = 0; // Offset 0
    	lock.l_len = 0; // Lock the entire file

    	// Try to acquire the lock in blocking mode
    	if (fcntl(studentFileDescriptor, F_SETLKW, &lock) == -1) {
        	perror("Error locking the file");
       	 	close(studentFileDescriptor);
        	exit(EXIT_FAILURE);
    	}

	struct student_struct student1, student;
	char StudentList[1000];
	StudentList[0]='\0';
	//getting the login id of all students to choose from
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

	//writing the login id of all the students avaliable blocked or unblocked
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
	//getting details of students whose login id is provided by user
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

			//unlocking
			lock.l_type = F_UNLCK;
    			if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
    			}	

			close(studentFileDescriptor);
			readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
			return true;
		}
	}
	strcpy(writeBuffer,"Wrong Option!\n Press any key to exit\n");
	//unlocking
        lock.l_type = F_UNLCK;
        if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
          	perror("Error releasing the lock");
        }
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
	readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	close(studentFileDescriptor);
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        return false;
}

//view faculty details
bool view_faculty_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int facultyFileDescriptor = open("FACULTY_FILE", O_RDONLY);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while opening file");
                return false;
        }

	struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK; // Read lock
        lock.l_whence = SEEK_SET; // Start from the beginning of the file
        lock.l_start = 0; // Offset 0
        lock.l_len = 0; // Lock the entire file

        // Try to acquire the lock in blocking mode
        if (fcntl(facultyFileDescriptor, F_SETLKW, &lock) == -1) {
                perror("Error locking the file");
                close(facultyFileDescriptor);
                exit(EXIT_FAILURE);
        }

        struct faculty_struct faculty1, faculty;
        char FacultyList[1000];
        FacultyList[0]='\0';
	//giving the list of faculty id avaliable
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
	bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	printf("%s\n",readBuffer);
        char login[20];

        strcpy(login, readBuffer);
	printf("%s\n",login);
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
	//searching the faculty with the id and returning the result
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

			//unlocking
			lock.l_type = F_UNLCK;
    			if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
    			}
			close(facultyFileDescriptor);
                        return true;
                }
        }
        strcpy(writeBuffer,"Wrong Option!\n Press any key to exit\n");
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	//unlocking
    	lock.l_type = F_UNLCK;
     	if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
         	perror("Error releasing the lock");
    	}
	close(facultyFileDescriptor);
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        return false;
}


//function to block the student
bool block_student(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        int studentFileDescriptor = open("STUDENT_FILE", O_RDWR, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
	
	//write lock
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
        char StudentList[1000];
        StudentList[0]='\0';
	//getting all the students ID to choose from
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
	
	//blocking the student
	while(read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student1.login, login) == 0 && student1.active == true){
			//when ID found to be blocked
			student1.active = false;
			lseek(studentFileDescriptor, -1 * sizeof(struct student_struct), SEEK_CUR);
			write(studentFileDescriptor, &student1, sizeof(struct student_struct));
			strcpy(writeBuffer, STUDENT_BLOCKED);
			writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
			readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
			//unlocking
			lock.l_type = F_UNLCK;
    			if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
    			}
			close(studentFileDescriptor);
			return true;
		}
	}
	//if STUDENT already blocked or not blocked
	strcpy(writeBuffer, STUDENT_NOT_BLOCKED);
	//uncloking file
	lock.l_type = F_UNLCK;
    	if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
	close(studentFileDescriptor);
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
	return false;
}


//function to activate blocked student
bool activate_student(int connectionFileDescriptor){
        ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
	//opening the file in Read Write Mode
        int studentFileDescriptor = open("STUDENT_FILE", O_RDWR, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
	// write lock
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
        char StudentList[1000];
        StudentList[0]='\0';
	//giving ID to be Activated from the list to choose from
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

	//searching the login id and activating them
        while(read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student1.login, login) == 0){
                        student1.active = true;
			lseek(studentFileDescriptor, -1 * sizeof(struct student_struct), SEEK_CUR);
                        write(studentFileDescriptor, &student1, sizeof(struct student_struct));
			strcpy(writeBuffer, STUDENT_ACTIVATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        // unlocking
			lock.l_type = F_UNLCK;
    			if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        			perror("Error releasing the lock");
   			}
			close(studentFileDescriptor);
		       	return true;
                }
        }
	//if id not found in blocked list
        strcpy(writeBuffer, STUDENT_NOT_ACTIVATED);
	
	// unlocking
        lock.l_type = F_UNLCK;
        if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
         	perror("Error releasing the lock");
      	}
	close(studentFileDescriptor);
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        return false;
}

//modify Student Details
bool modify_student_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        //opening the file in Read Write Mode
	int studentFileDescriptor = open("STUDENT_FILE", O_RDWR, 0777);
        if (studentFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }

	//write lock
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
        struct student_struct student1, student;
        char StudentList[1000];
        StudentList[0]='\0';
	//giving all the student id to be modified as choice
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
        strcat(writeBuffer, STUDENT_UPDATE);

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
	//searching the requested ID and modifying it.
	while (read(studentFileDescriptor, &student1, sizeof(struct student_struct)) == sizeof(struct student_struct)){
		if(strcmp(student1.login,login)==0){
			lseek(studentFileDescriptor, -1 * sizeof(struct student_struct), SEEK_CUR);
			//to update name
        		bzero(writeBuffer, sizeof(writeBuffer));
        		bzero(readBuffer, sizeof(readBuffer));

        		strcpy(writeBuffer, STUDENT_NAME);
        		strcat(writeBuffer, CHOICE);
       			writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        		if(strcmp(readBuffer,"y")==0){
                		bzero(writeBuffer, sizeof(writeBuffer));
                		bzero(readBuffer, sizeof(readBuffer));

                		strcpy(writeBuffer, UPDATE);
                		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                		strcpy(student1.name,readBuffer);
        		}

        		//to update address
        		bzero(writeBuffer, sizeof(writeBuffer));
        		bzero(readBuffer, sizeof(readBuffer));
        		strcpy(writeBuffer, STUDENT_ADDRESS);
        		strcat(writeBuffer, CHOICE);
        		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        		if(strcmp(readBuffer,"y")==0){
                		bzero(writeBuffer, sizeof(writeBuffer));
                		bzero(readBuffer, sizeof(readBuffer));

                		strcpy(writeBuffer, UPDATE);
                		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                		strcpy(student1.address,readBuffer);
        		}

       			//to update email
       			bzero(writeBuffer, sizeof(writeBuffer));
       			bzero(readBuffer, sizeof(readBuffer));
        		strcpy(writeBuffer, STUDENT_EMAIL);
        		strcat(writeBuffer, CHOICE);
        		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        		if(strcmp(readBuffer,"y")==0){
                		bzero(writeBuffer, sizeof(writeBuffer));
                		bzero(readBuffer, sizeof(readBuffer));

                		strcpy(writeBuffer, UPDATE);
                		writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                		readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                		strcpy(student1.email,readBuffer);
        		}

        		bzero(writeBuffer, sizeof(writeBuffer));
        		bzero(readBuffer, sizeof(readBuffer));

			write(studentFileDescriptor, &student1, sizeof(struct student_struct));
                        // unlocking
                        lock.l_type = F_UNLCK;
                        if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
                                perror("Error releasing the lock");
                        }
			close(studentFileDescriptor);
                        strcpy(writeBuffer, UPDATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        return true;
		}
	}
	//if ID not found
	// unlocking
        lock.l_type = F_UNLCK;
   	if (fcntl(studentFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
    	}
      	close(studentFileDescriptor);
        strcpy(writeBuffer, NOT_UPDATED);
	writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        return true;
}


//modify faculty details
bool modify_faculty_details(int connectionFileDescriptor){
	ssize_t readBytes, writeBytes;
        char readBuffer[1000], writeBuffer[1000];
        //opening the faculty file in read write mode
	int facultyFileDescriptor = open("FACULTY_FILE", O_RDWR, 0777);
        if (facultyFileDescriptor == -1)
        {
                perror("Error while opening student file");
                return false;
        }
	//write lock
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
        char FacultyList[1000];
        FacultyList[0]='\0';
	//giving a list of faculty id to choose from
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
        strcat(writeBuffer, FACULTY_UPDATE);

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
	//searching and modifying the faculty details
        while (read(facultyFileDescriptor, &faculty1, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
                if(strcmp(faculty1.login,login)==0){
			//we found the faculty
                        lseek(facultyFileDescriptor, -1 * sizeof(struct faculty_struct), SEEK_CUR);
                        //to update name
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

                        strcpy(writeBuffer, FACULTY_NAME);
                        strcat(writeBuffer, CHOICE);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if(strcmp(readBuffer,"y")==0){
                                bzero(writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));

                                strcpy(writeBuffer, UPDATE);
                                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                                strcpy(faculty1.name,readBuffer);
                        }
			
			//to update department
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

                        strcpy(writeBuffer, FACULTY_DEPARTMENT);
                        strcat(writeBuffer, CHOICE);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if(strcmp(readBuffer,"y")==0){
                                bzero(writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));

                                strcpy(writeBuffer, UPDATE);
                                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                                strcpy(faculty1.department,readBuffer);
                        }

			//to update designation
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

                        strcpy(writeBuffer, FACULTY_DESIGNATION);
                        strcat(writeBuffer, CHOICE);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if(strcmp(readBuffer,"y")==0){
                                bzero(writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));

                                strcpy(writeBuffer, UPDATE);
                                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                                strcpy(faculty1.designation,readBuffer);
                        }

                        //to update address
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));
                        strcpy(writeBuffer, FACULTY_ADDRESS);
                        strcat(writeBuffer, CHOICE);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if(strcmp(readBuffer,"y")==0){
                                bzero(writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));

                                strcpy(writeBuffer, UPDATE);
                                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                                strcpy(faculty1.address,readBuffer);
                        }

                        //to update email
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));
                        strcpy(writeBuffer, FACULTY_EMAIL);
                        strcat(writeBuffer, CHOICE);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                        if(strcmp(readBuffer,"y")==0){
                                bzero(writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));

                                strcpy(writeBuffer, UPDATE);
                                writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                                readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
                                strcpy(faculty1.email,readBuffer);
                        }

                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));

                        write(facultyFileDescriptor, &faculty1, sizeof(struct student_struct));
                        // unlocking
                        lock.l_type = F_UNLCK;
                        if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
                                perror("Error releasing the lock");
                        }
			close(facultyFileDescriptor);
                        strcpy(writeBuffer, UPDATED);
                        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
                        return true;
                }
        }
	//faculty id not found
	// unlocking
     	lock.l_type = F_UNLCK;
     	if (fcntl(facultyFileDescriptor, F_SETLK, &lock) == -1) {
        	perror("Error releasing the lock");
      	}
        strcpy(writeBuffer, NOT_UPDATED);
        writeBytes = write(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        readBytes = read(connectionFileDescriptor, writeBuffer, strlen(writeBuffer));
        return true;
}

#endif


