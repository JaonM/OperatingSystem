#ifndef PCH_H
#define PCH_H
#include "stdafx.h"
#include "System.h"
//#include "pthread.h"

class Memory;
class System;
class MemoryBlock;

/*���̿��ƿ���*/
class PCB {
public :
	PCB(int id, std::string name,int size,System*&);
//	virtual ~PCB();
//	virtual void operator() ()=0;		//��������
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
	 virtual void costMemory(Memory* memory,std::string strategy); //ռ���ڴ�
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
	int status;				//0-���� 1-ִ�� 2-����
	int priority;				//���ȼ�
	int startTime;		//���̿�ʼʱ��
	int size;				//����ռ�ô�С
	std::string name;	//������
	int serveTime;	//��������ʱ��
	int finishTime;	//���̽���ʱ��
	int turnoverTime;	//������תʱ��
	int weightTurnoverTime;	//��Ȩ��תʱ��

	int actualServeTime;		//������������ʱ��
	System*& system;
};

/*����̨������,�˽��̸������û�����*/
class ConsolePCB:public PCB{
public:
	ConsolePCB(int id, std::string name, int size,System*& system);
//	virtual void costMemory(Memory*);
	virtual void operator()();
	
};

/*ϵͳ������,���ȼ����*/
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