#ifndef PCH_H
#define PCH_H
#include "stdafx.h"
#include "System.h"
//#include "pthread.h"

class Memory;
class System;
class MemoryBlock;

/*进程控制块类*/
class PCB {
public :
	PCB(int id, std::string name,int size,System*&);
//	virtual ~PCB();
//	virtual void operator() ()=0;		//函数对象
	int getPID();
	void setStatus(int status);
	int getStatus();
	int getStartTime();
	void setPriority(int priority);
	int getPriority();
	int getSize();
	int getServeTime();
	int getFinishTime();
	int getTurnoverTime();

	int getWeightTurnoverTime();
	void setFinishTime(int);
	void setTurnOverTime(int);
	 virtual void costMemory(Memory* memory,std::string strategy); //占用内存
	 std::string getName();

	 void setActualServeTime(int);
	 int getActualServeTime();
	/* void Start()
	 {
		 pthread_create(&m_ThreadID, NULL, ThreadFunc, this);
	 }

	 static void* ThreadFunc(void* pth)
	 {
		 PCB* p = static_cast<PCB*>(pth);
		 p->Run();
	 }*/

//	 virtual void Run() = 0;
protected:
	int PID;
	int status;				//0-就绪 1-执行 2-阻塞
	int priority;				//优先级
	int startTime;		//进程开始时间
	int size;				//进程占用大小
	std::string name;	//进程名
	int serveTime;	//进程运行时间
	int finishTime;	//进程结束时间
	int turnoverTime;	//进程周转时间
	int weightTurnoverTime;	//带权周转时间

	int actualServeTime;		//进程真正运行时间
	System*& system;
};

/*控制台进程类,此进程负责与用户交互*/
class ConsolePCB:public PCB{
public:
	ConsolePCB(int id, std::string name, int size,System*& system);
//	virtual void costMemory(Memory*);
	virtual void operator()();
	
};

/*系统进程类,优先级最高*/
class SystemPCB :public PCB {
public:
	SystemPCB(int id, std::string name, int size,System*& system);
//	virtual void costMemory(Memory*);
};

class UserPCB :public PCB{
public:
	UserPCB(int id, string name, int size, System*&);

};

class FileSystemPCB :public PCB{
public:
	FileSystemPCB(int id, string name, int size, System*&);

};
#endif