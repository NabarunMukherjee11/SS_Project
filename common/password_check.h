#ifndef PASSWORD_CHECKER
#define PASSWORD_CHECKER

#include "../structModels/student_struct.h"
#include "../structModels/faculty_struct.h"


bool faculty_password_checker(char *login_id, char *password);
bool student_password_checker(char *login_id, char *password);

bool faculty_password_checker(char *login_id, char *password){
        int fileDescriptor = open("FACULTY_FILE", O_RDONLY);

        if (fileDescriptor == -1) {
                perror("Error opening file");
                return 1;
        }

        struct faculty_struct faculty;

        while (read(fileDescriptor, &faculty, sizeof(struct faculty_struct)) == sizeof(struct faculty_struct)){
                if (strcmp(faculty.login, login_id) == 0) {
                        if(strcmp(faculty.password, password) == 0){
                                printf("Password match\n");
				return true;
                        }
                }
        }
        return false;
}

bool student_password_checker(char *login_id, char *password){
        int fileDescriptor = open("STUDENT_FILE", O_RDONLY);

        if (fileDescriptor == -1) {
                perror("Error opening file");
                return false;
        }

        struct student_struct student;

        while (read(fileDescriptor, &student, sizeof(struct student_struct)) == sizeof(struct student_struct)){
                if (strcmp(student.login, login_id) == 0) {
			if(student.active == false){
				return false;
			}
                        if(strcmp(student.password, password) == 0){
                                printf("Password match\n");
                                return true;
                        }
                }
        }
        return false;
}


#endif
