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

/*内存块结构*/
/*head	|	left	tag	size	right		|
			|										|
			|			space					|
			|										|
 foot		|	up									|				
*/
class MemoryBlock{
public:
	MemoryBlock(int,int,int);
	MemoryBlock* left;		//前驱结点
	MemoryBlock* right;	//后驱结点
	MemoryBlock *up;			//指向head区域
	MemoryBlock* FootLocate(MemoryBlock*);	//定位foot区域位置

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
	int tag;				//0-空闲	1-占用
	int size;
	int free;			//剩余内存
	int nonfree;		//已用内存

	string processName;		//占用进程名
};
/*内存结构,分为许多内存块*/
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
	MemoryBlock* getAddress();	//返回内存首地址

	void showMemory();		//总体显示内存使用情况
	void showMemoryDetail();	//详细显示内存使用情况
private:
	const int size;			//内存大小
	int currentMemory;	//当前已用内存

	MemoryBlock* head;	//内存块首地址
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
	void output();	//输出系统当前状态
	void setStrategy(string strategy);
	string getStrategy();
	FileSystem* getFileSystem();

private:
	FileSystem* fileSystem;
	Memory* memory;
	std::string version;
	std::string  sysTime;
	std::vector<PCB*>* processQueue;		//进程队列
	std::vector<User>* userQueue;				//用户队列

	std::string currentUser;				//当前登陆用户			

	string memoryStrategy;		//内存分配策略
};
#endif