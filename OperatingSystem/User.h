#ifndef USER_H
#define USER_H

#include "stdafx.h"
using namespace std;
/*系统用户*/
class User {
public:
	User(int id, string name);
	int getId();
	string getName();
	void setAccount(string);
	string getAccount();
	void setPassword(string);
	string getPassword();
private:
	int id;
	string name;
	string account;
	string password;
};
#endif;