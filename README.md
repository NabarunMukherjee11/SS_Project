# SS_Project
#Online Admission Portal - Read Me

#Introduction
Welcome to the Online Admission Portal project. This document will guide you through the structure of the project and provide instructions on how to set it up and run it successfully. The portal is designed to cater to three types of users: ADMIN, STUDENT, and FACULTY.

#Project Structure
The project is organized into several directories and files:

1.Structure:

Contains Client.c and Server.c programs, which are essential for the functioning of the online portal.
Student data is stored in the "STUDENT FILE."
Faculty data is stored in the "FACULTY FILE."
Course details are maintained in "COURSE_LIST" and "COURSE_STUDENT" files.

#Common:

Houses common files for login and password checking that are used by all three types of users (ADMIN, STUDENT, FACULTY).
Functions:

This directory contains separate portal programs for each type of user:
"student_portal.h" for STUDENT functionalities.
"faculty_portal.h" for FACULTY functionalities.
"admin_portal.h" for ADMIN functionalities.

#StructModels:

Includes structures for managing course, faculty, and student data. These structures are vital for the core functionality of the portal.

#Prerequisites
To successfully run the Online Admission Portal, ensure that you have the following prerequisites:

A C compiler installed on your system.
A working command-line interface.
Running the Portal
Follow these steps to run the Online Admission Portal:

1. Open a terminal window. Navigate to the "Structure" directory: "cd Structure"
2. Compile and run the server program (Server.c) to host the portal: "gcc -o Server Server.c"
                                                                   : "./Server" 
3. In a separate terminal window, navigate to the "Structure" directory again: cd "Structure"
4. Compile and run the client program (Client.c) to access the portal: "gcc -o Client Client.c"
                                                                     : "./Client"
5. The portal should now be accessible via the client program, and you can log in as ADMIN, STUDENT, or FACULTY, using the login and password. (the password for ADMIN is "admin")

Once the portal is up and running, you can use the respective portal programs from the "Functions" directory to perform actions according to your user type:

For ADMIN functionalities, use "admin_portal.h"
For STUDENT functionalities, use "student_portal.h"
For FACULTY functionalities, use "faculty_portal.h"
The portal provides various features for each user type, such as managing courses, viewing student and faculty information, and handling admissions.

