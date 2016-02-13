// OperatingSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PCB.h"
#include "System.h"
#include <sstream>
#include "tinyxml.h"
#include "XMLParser.h"


using namespace std;

boolean login(System*, string, string);
int main()
{
	//初始化系统
	//System* system=XMLParser::parseConfiguration("Configuration.xml");
	//cout << system<<endl;
	//cout << system->getMemory().getSize() << endl;
	System* p=nullptr;
	//XMLParser::parseConfiguration("Configuration.xml", p);
	//cout<<p->getMemory()->getSize() << endl;
	//cout << p->getMemory()->getSize() << endl;
	//cout << p->getMemory()->getSize() << endl;
	if (XMLParser::parseConfiguration("Configuration.xml", p)) {
		cout << "系统加载成功" << endl;
		cout << "系统当前时间: " << p->getSysTime() << endl;
		cout << "系统版本: " << p->getVersion() << endl;
		cout << "系统内存情况: " << endl;
		cout << "共: " << p->getMemory()->getSize() << "KB " << "已用: " << p->getMemory()->getCurrentMemory() << "KB " << "剩余: " << p->getMemory()->getLeftMemory() << "" << endl;

		MemoryBlock *t = p->getMemory()->getAddress();
		while (t != nullptr) {
			cout << "id: " << t->getId() << " 内存块大小为: " << t->getSize() << " 内存地址: " <<t << endl;
			t = t->right;
		}
	}

	/*Memory* m=new Memory(100);
	MemoryBlock *p = m->getAddress();
	while (p!= nullptr) {
		cout << "id: " << p->getId() << " 内存块大小为: " << p->getSize() << "内存地址: "<<p<<endl;
		p = p->right;
	}
	cout << "当前已用内存: " << m->getCurrentMemory ()<< endl;
	cout << "当前剩余内存: " << m->getLeftMemory() << endl*/;
	string userName;
	string password;

	cout << endl;

	while (true) {
		cout << "输入账号: ";
		cin >> userName;
		cout << "输入密码: ";

		cin >> password;

		if (login(p,userName,password)) {
			cout << endl;
			cout << "=================================>>欢迎" << userName << "<<====================================" << endl;
			SYSTEMTIME time;
			GetLocalTime(&time);
//			cout << "系统当前时间: " << endl;
//			cout << time.wYear << "年" << time.wMonth << "月" << time.wDay << "号 " << time.wHour << "点" << time.wMinute<<"分"<<endl;
			p->setCurrentUser(userName);
			break;
		}
		else {
			cout << "账号或密码错误" << endl;
		}
	}

	cout << "输入help查看可输入的命令" << endl;

	/*新开进程接受命令,计算初始的内存消耗,将进程加入管理队列当中*/
	SystemPCB systemPCB(1, "System Process",20, p);
	ConsolePCB consolePCB(2,"Console Process",8, p);
	UserPCB userPCB(4,"User "+ p->getCurrentUser() + " Process", 2, p);
//	FileSystemPCB fileSystemPCB(3, "File System Process", 3, p);
//	p->getProcessQueue().push(&systemPCB);
//	p->getProcessQueue().push(&consolePCB);
	thread t(consolePCB);
	
	t.join();
	exit(0);
	return 0;
}



/*用户登录*/
boolean login(System* sys,string account, string password) {
	int userSize = sys->getUserQueue()->size();
	
	for (vector<User>::iterator it = sys->getUserQueue()->begin(); it != sys->getUserQueue()->end();it++){
//		User user = sys->getUserQueue()->front();
		if (it->getAccount() == account&&it->getPassword() == password) {
			sys->setCurrentUser(it->getName());
			return true;
		}
	}
	return false;
}