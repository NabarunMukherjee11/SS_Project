#ifndef CONSTANTS
#define CONSTANTS

//Server Page
#define INITIAL_PROMPT "*********** IIITB ACADEMIA PORTAL ***********\nPlease specify by pressing:\n1 for ADMIN login\n2 for FACULTY login\n3 for STUDENT login\nPress any other number to exit\n"


//Admin Page
#define ADMIN_LOGIN_WELCOME "*********** WELCOME ADMIN ***********\nPlease authenticate with ADMIN password.\n Password\n"
#define ADMIN_PASSWORD "admin"
#define ADMIN_LOGIN_SUCCESS "Welcome to Admin Portal. You are now logged in.\n"
#define ADMIN_MENU "Welcome to Admin Menu.\nChoose the option you want to proceed with\n1. ADD STUDENT\n2. VIEW STUDENT DETAILS\n3.ADD FACULTY\n4. VIEW FACULTY DETAILS\n5. ACTIVATE STUDENT\n6. BLOCK STUDENT\n7. MODIFY STUDENT DETAILS\n8. MODIFY FACULTY DETAILS\n9. LOGOUT AND EXIT\nCHOOSE THE OPTION\n"
#define ADMIN_LOGOUT "Admin has been logged out\n"
#define ADMIN_AGAIN "You have entered wrong option\n"
#define STUDENT_NAME "Enter student name\n"
#define STUDENT_AGE "Enter student age\n"
#define STUDENT_ADDRESS "Enter the students address\n"
#define STUDENT_EMAIL "Set an Email for the student\n"
#define STUDENT_LOGIN "Set the LOGIN ID for the student\n"
#define STUDENT_PASSWORD "Set a Password for the student\n"
#define STUDENT_ADDED "Student added successfully\nEnter any key to go to ADMIN MENU\n"
#define FACULTY_NAME "Enter the Faculty Name\n"
#define FACULTY_DESIGNATION "Enter the Faculty Designation\n"
#define FACULTY_DEPARTMENT "Enter the Faculty Department\n"
#define FACULTY_EMAIL "Enter the Faculty Email id\n"
#define FACULTY_ADDRESS "Enter the Faculty Address\n"
#define FACULTY_LOGIN "Enter the Faculty Login id\n"
#define FACULTY_PASSWORD "Set the Faculty Password\n"
#define FACULTY_ADDED "Faculty added successfully\nEnter any key to go to ADMIN MENU\n"
#define STUDENT_DETAILS "Enter the Login ID whose details you want\n"
#define STUDENT_BLOCK "Enter the Login ID of the student to be BLOCKED\n(you can always UNBLOCK them)\n"
#define STUDENT_BLOCKED "The Student has been BLOCKED\nPress any key to exit!\n"
#define STUDENT_NOT_BLOCKED "The Student has NOT been BLOCKED\nWrong LOGIN ID!\nPress any key to exit!\n"
#define STUDENT_ACTIVE "Enter the Login ID of the student to be ACTIVATED\n"
#define STUDENT_ACTIVATED "The Student has been ACTIVATED\nPress any key to exit!\n"
#define STUDENT_NOT_ACTIVATED "The Student has NOT been ACTIVATED\nWrong LOGIN ID!\nPress any key to exit!\n"


//Faculty Page
#define F_FACULTY_LOGIN_WELCOME "*********** WELCOME FACULTY ***********\nPlease authenticate with LOGIN ID and PASSWORD.\n LOGIN_ID:\n"
#define F_FACULTY_PASSWORD "PASSWORD:\n"
#define F_FACULTY_LOGIN_SUCCESS "****** Welcome to Faculty Portal ******\nYou are now logged in\n"
#define F_FACULTY_MENU "Choose the option you want to proceed with\n1. VIEW  OFFERING COURSE\n2. ADD NEW COURSE\n3. REMOVE COURSE FROM CATALOG\n4. UPDATE COURSE DETAILS\n 5. CHANGE PASSWORD\n6. LOGOUT & EXIT\n"
#define F_FACULTY_LOGOUT "FACULTY has been logged out\n"
#define F_FACULTY_AGAIN "You have entered wrong option\n"

//Student Page
#define S_STUDENT_LOGIN_WELCOME "*********** WELCOME STUDENT ***********\nPlease authenticate with LOGIN ID and PASSWORD.\n LOGIN_ID:\n"
#define S_STUDENT_PASSWORD "PASSWORD:\n"
#define S_STUDENT_LOGIN_SUCCESS "****** Welcome to Student Portal ******\nYou are now logged in\n"
#define S_STUDENT_MENU "Choose the option you want to proceed with\n1. VIEW ALL COURSES\n2. ENROLL NEW COURSE\n3. DROP COURSE\n4. VIEW COURSE DETAILS\n 5. CHANGE PASSWORD\n6. LOGOUT & EXIT\n"
#define S_STUDENT_LOGOUT "STUDENT has been logged out\n"
#define S_STUDENT_AGAIN "You have entered wrong option\n"

#endif
