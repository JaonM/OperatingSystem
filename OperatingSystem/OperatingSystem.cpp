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
	//��ʼ��ϵͳ
	//System* system=XMLParser::parseConfiguration("Configuration.xml");
	//cout << system<<endl;
	//cout << system->getMemory().getSize() << endl;
	System* p=nullptr;
	//XMLParser::parseConfiguration("Configuration.xml", p);
	//cout<<p->getMemory()->getSize() << endl;
	//cout << p->getMemory()->getSize() << endl;
	//cout << p->getMemory()->getSize() << endl;
	if (XMLParser::parseConfiguration("Configuration.xml", p)) {
		cout << "ϵͳ���سɹ�" << endl;
		cout << "ϵͳ��ǰʱ��: " << p->getSysTime() << endl;
		cout << "ϵͳ�汾: " << p->getVersion() << endl;
		cout << "ϵͳ�ڴ����: " << endl;
		cout << "��: " << p->getMemory()->getSize() << "KB " << "����: " << p->getMemory()->getCurrentMemory() << "KB " << "ʣ��: " << p->getMemory()->getLeftMemory() << "" << endl;

		MemoryBlock *t = p->getMemory()->getAddress();
		while (t != nullptr) {
			cout << "id: " << t->getId() << " �ڴ���СΪ: " << t->getSize() << " �ڴ��ַ: " <<t << endl;
			t = t->right;
		}
	}

	/*Memory* m=new Memory(100);
	MemoryBlock *p = m->getAddress();
	while (p!= nullptr) {
		cout << "id: " << p->getId() << " �ڴ���СΪ: " << p->getSize() << "�ڴ��ַ: "<<p<<endl;
		p = p->right;
	}
	cout << "��ǰ�����ڴ�: " << m->getCurrentMemory ()<< endl;
	cout << "��ǰʣ���ڴ�: " << m->getLeftMemory() << endl*/;
	string userName;
	string password;

	cout << endl;

	while (true) {
		cout << "�����˺�: ";
		cin >> userName;
		cout << "��������: ";

		cin >> password;

		if (login(p,userName,password)) {
			cout << endl;
			cout << "=================================>>��ӭ" << userName << "<<====================================" << endl;
			SYSTEMTIME time;
			GetLocalTime(&time);
//			cout << "ϵͳ��ǰʱ��: " << endl;
//			cout << time.wYear << "��" << time.wMonth << "��" << time.wDay << "�� " << time.wHour << "��" << time.wMinute<<"��"<<endl;
			p->setCurrentUser(userName);
			break;
		}
		else {
			cout << "�˺Ż��������" << endl;
		}
	}

	cout << "����help�鿴�����������" << endl;

	/*�¿����̽�������,�����ʼ���ڴ�����,�����̼��������е���*/
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



/*�û���¼*/
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