#ifndef SYSTEM_H
#define SYSTEM_H

#include "stdafx.h"
#include <ctime>
#include <queue>
#include "User.h"
#include "PCB.h"
#include <vector>
#include "FileSystem.h"

using namespace std;

class PCB;

class Time {
public:
	static void delay(int time);
};

/*�ڴ��ṹ*/
/*head	|	left	tag	size	right		|
			|										|
			|			space					|
			|										|
 foot		|	up									|				
*/
class MemoryBlock{
public:
	MemoryBlock(int,int,int);
	MemoryBlock* left;		//ǰ�����
	MemoryBlock* right;	//�������
	MemoryBlock *up;			//ָ��head����
	MemoryBlock* FootLocate(MemoryBlock*);	//��λfoot����λ��

	int getTag();
	void setTag(int tag);
	int getSize();
	int getId();
	void setFree(int);
	int getFree();
	void setNonFree(int);
	int getNonFree();
	void setProcessName(string);
	string getProcessName();
private:
	int id;
	int tag;				//0-����	1-ռ��
	int size;
	int free;			//ʣ���ڴ�
	int nonfree;		//�����ڴ�

	string processName;		//ռ�ý�����
};
/*�ڴ�ṹ,��Ϊ����ڴ��*/
class Memory {
public:
	Memory(int size);
	Memory(const Memory& src);
	int getSize();
//	virtual ~Memory();
	int getCurrentMemory();
	void setCurrentMemory(int value);
	boolean isOutOfMemory();
	int getLeftMemory();
	MemoryBlock* getAddress();	//�����ڴ��׵�ַ

	void showMemory();		//������ʾ�ڴ�ʹ�����
	void showMemoryDetail();	//��ϸ��ʾ�ڴ�ʹ�����
private:
	const int size;			//�ڴ��С
	int currentMemory;	//��ǰ�����ڴ�

	MemoryBlock* head;	//�ڴ���׵�ַ
};

class System {
public:
//	virtual ~System();
	System(string sysTime,int memorySize);
	std::vector<PCB*>* getProcessQueue();
	std::vector<User>* getUserQueue();
	void setVersion(string);
	string getVersion();
	void setMemory(Memory);
	Memory* getMemory();
	void setSysTime(string);
	string getSysTime();

	void setCurrentUser(string);
	string getCurrentUser();
	void output();	//���ϵͳ��ǰ״̬
	void setStrategy(string strategy);
	string getStrategy();
	FileSystem* getFileSystem();

private:
	FileSystem* fileSystem;
	Memory* memory;
	std::string version;
	std::string  sysTime;
	std::vector<PCB*>* processQueue;		//���̶���
	std::vector<User>* userQueue;				//�û�����

	std::string currentUser;				//��ǰ��½�û�			

	string memoryStrategy;		//�ڴ�������
};
#endif