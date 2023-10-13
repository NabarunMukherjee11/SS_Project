#ifndef LOGIN_FACULTY
#define LOGIN_FACULTY

#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`
#include "../../Functions/server_const.h"
#include "../password_check.h"

bool login_faculty(bool isAdmin, int connectionFileDescriptor, char* faculty_id, char* pass_buffer);

bool login_faculty(bool isAdminn, int connectionFileDescriptor, char* faculty_id, char* pass_buffer){
	if(faculty_password_checker(faculty_id, pass_buffer)){
		return true;
	}
	return false;
}


#endif





