#ifndef STRUCT_COURSE
#define STRUCT_COURSE

struct course_struct{
	int course_flag;
	char courseName[20];
	char courseID[20];
	char facultyID[20];
	int SeatsAvaliable;
	char totalSeats[10];
};

struct course_student_struct{
	int cs_flag;
	char courseID[20];
	char studentID[20];
};

#endif
