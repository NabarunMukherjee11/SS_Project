#ifndef LOGIN_STUDENT
#define LOGIN_STUDENT

#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`
#include "../../Functions/server_const.h"
#include "../password_check.h"

bool login_student(bool isAdmin, int connectionFileDescriptor, char* student_id, char* passBuffer);

bool login_student(bool isAdminn, int connectionFileDescriptor, char* student_id, char* passBuffer){
        if(student_password_checker(student_id, passBuffer)){
                return true;
        }
        return false;
}


#endif

