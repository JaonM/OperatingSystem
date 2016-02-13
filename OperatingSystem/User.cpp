#include "stdafx.h"
#include "User.h"

using namespace std;

User::User(int id, string name) :id(id), name(name){

}

void User::setAccount(string account){
	this->account = account;
}
string User::getAccount(){
	return this->account;
}
void User::setPassword(string password){
	this->password = password;
}
string User::getPassword(){
	return this->password = password;
}
string User::getName(){
	return this->name;
}