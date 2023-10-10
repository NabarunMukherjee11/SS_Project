#include "../common/Login/login_ad.h"

bool admin_portal(int connectionFileDescriptor);

bool admin_portal(int connectionFileDescriptor){
	if(login_admin(true, connectionFileDescriptor)){
	return true;}}

