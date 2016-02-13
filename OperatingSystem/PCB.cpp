#include "stdafx.h"
#include "PCB.h"
#include <time.h>
#include <map>
#include <cmath>

#define random(x){rand()%x}

using namespace std;

/*PCB��*/
PCB::PCB(int id,std::string name,int size,System*& system):PID(id),name(name),size(size),system(system) {
	/*PID=id;
	this->name = name;*/
	//SYSTEMTIME time;
	//GetLocalTime(&time);
	startTime = random(20);
//	size = sizeof(this);
	costMemory(system->getMemory(),system->getStrategy());
	system->getProcessQueue()->push_back(this);
	int temp0 = random(10);
	serveTime = abs(temp0 - startTime) == 0 ? 1 : abs(temp0 - startTime);
	int temp = random(5);
	finishTime = startTime + serveTime + temp;
	this->turnoverTime = finishTime - startTime;
	this->weightTurnoverTime = turnoverTime/serveTime;
	this->actualServeTime = 0;
}

int PCB::getPID() {
	return this->PID;
}
void PCB::setPriority(int priority) {
	this->priority = priority;
}
void PCB::setStatus(int status) {
	this->status = status;
}
int PCB::getStatus() {
	return this->status;
}
int PCB::getPriority() {
	return this->priority;
}
int PCB::getStartTime() {
	return this->startTime;
}
int PCB::getSize() {
	return this->size;
}
int PCB::getServeTime() {
	//this->serveTime = time(NULL) - startTime;
//	serveTime = random(10);
	return serveTime;
}
int PCB::getFinishTime() {
	
	return finishTime;
}
int PCB::getTurnoverTime(){
	return this->finishTime-startTime;
}
int PCB::getWeightTurnoverTime(){
	return this->weightTurnoverTime;
}
string PCB::getName(){
	return this->name;
}

void PCB::setFinishTime(int finishTime){
	this->finishTime = finishTime;
}
void PCB::setTurnOverTime(int turnOverTime) {
	this->turnoverTime = turnOverTime;
}

void PCB::setActualServeTime(int actualTime) {
	this->actualServeTime = actualTime;
}
int PCB::getActualServeTime(){
	return actualServeTime;
}

void PCB::costMemory(Memory* memory,std::string strategy){
	if (strategy == "FIRST_FIT") {		/*�״���Ӧ�㷨*/
	//	cout << sizeof(this) << endl;
		MemoryBlock* p = memory->getAddress();
		while (p != nullptr){
			if (p->getFree() >= size&&p->getTag() == 0) {
				break;
			}
			p = p->right;
		}
		if (p != nullptr) {
			p->setNonFree(size);
			p->setTag(1);
			p->setProcessName(name);
		}
		else{
			cout << "�����ڴ�ʧ��" << endl;
		}
	}
	else if (strategy == "BEST_FIT") {			/*���ƥ���㷨*/
		map<int, MemoryBlock*> m;
		MemoryBlock* p = memory->getAddress();
		p = p->right;
		int minDistance = abs(p->getFree() - size);
		while (p != nullptr) {
			if (p->getTag() == 0 && size <= p->getFree()) {
				int distance = p->getFree() - size;
//				cout << "distance: " <<distance<< endl;
				if (distance < minDistance) {
					minDistance = distance;
				}
				m.insert(pair<int,MemoryBlock*>(distance,p));
			}
			p = p -> right;
		}
		map<int, MemoryBlock*>::iterator lt = m.find(minDistance);
//		cout << "min: " << lt->first << endl;
		if (lt != m.end()) {
			p = lt->second;
			if (p->getFree() != 0) {
				p->setNonFree(size);
				p->setTag(1);
				p->setProcessName(name);
			}
			else {
				cout << "�ڴ����ʧ��" << endl;
			}
		}
		else {
			cout << " �ڴ����ʧ��" << endl;
		}
	}
}

/*ConsolePCB��*/
//�����ڴ�����㷨�����ڴ�		�״���Ӧ�ڴ�����㷨
/*void ConsolePCB::costMemory(Memory* memory) {
	cout << sizeof(this) << endl;
	MemoryBlock* p = memory->getAddress();
	while (p != nullptr){
	if (p->getFree() >= size&&p->getTag() == 0) {
	break;
	}
	p = p->right;
	}
	if (p != nullptr) {
	p->setNonFree(size);
	p->setTag(1);
	}
	}*/
ConsolePCB::ConsolePCB(int id, string name, int size, System*& system) :PCB(id, name, size, system)/*,memory(memory)*/{
	//	costMemory(system->getMemory());
	setPriority(8);
}

bool sortByStartTime(PCB*, PCB*);

void ConsolePCB::operator()(){
	cout << "Console Process running" << endl;
	//	cout << memory.getSize() << endl;
	//	cout <<"��ʼʱ��: "<< getStartTime() << endl;
	string order;
	boolean isRunning = true;
	while (isRunning) {
		cout << "[" << system->getCurrentUser() << "] " << "/"<<system->getFileSystem()->currentFile->getName() << " $:";
		cin >> order;

		if (order == "shutdown" || order == "shutDown") {
			isRunning = false;
		}
		else if (order == "q"){
			cout << endl;
			continue;
		}
		else if (order == "system") {
			cout << endl;
			system->output();
			cout << endl;

		}
		else if (order == "memory"){
			system->getMemory()->showMemory();
			cout << endl;

		}
		else if (order == "md"){
			system->getMemory()->showMemoryDetail();
			cout << endl;

		}
		else if (order == "FCFS" || order == "fcfs") {
			cout << "���̵��� �����ȷ���ģ��" << endl;
			vector<PCB*>* v = system->getProcessQueue();
			sort(v->begin(), v->end(), sortByStartTime);
			int avTurnOverTime = 0;
			int avWeightTurnOverTime = 0;
			for (vector<PCB*>::iterator it = v->begin(); it != v->end(); it++) {
				//cout << (*it)->getName() << " ����ʱ��: " << (*it)->getStartTime()<<" ";
				avTurnOverTime += (*it)->getTurnoverTime();
				avWeightTurnOverTime += (*it)->getTurnoverTime();
				cout << "������: " << (*it)->getName() << "\t" << (*it)->getStartTime() << "\t" << (*it)->getServeTime() << "\t" << (*it)->getFinishTime() << "\t" << (*it)->getTurnoverTime()
					<< "\t" << (*it)->getWeightTurnoverTime() << endl;
			}
			 
			cout << "ƽ����תʱ��Ϊ: " << avTurnOverTime / v->size() << endl;
			cout << "��Ȩƽ����תʱ��Ϊ: " << avWeightTurnOverTime / v->size() << endl; 
			cout << endl;
		}
		else if (order == "timepiece") {/*ʱ��Ƭ��ת�㷨*/
			cout << "����ʱ��Ƭ��С" << endl;
			int piece;
			cin >> piece;
			int timer = 0;		/*��ʼʱ��*/
			bool isStop = false;
			vector<PCB*>* v = system->getProcessQueue();
			/*��ʼ��*/
			for (vector<PCB*>::iterator it = v->begin(); it != v->end(); it++){
				(*it)->setFinishTime(0);
				(*it)->setTurnOverTime(0);
			}
			sort(v->begin(), v->end(), sortByStartTime);

			vector<PCB*>::iterator it = v->begin();
			for (vector<PCB*>::iterator t = it; t != v->end(); t++) {
				cout << "������: " << (*t)->getName() << "����ʱ��: " << (*t)->getServeTime() << endl;
			}
			int finishCount = 0;
			while (!isStop) {
				for (vector<PCB*>::iterator t = it; t != v->end(); t++) {
					cout << "������: " << (*t)->getName() << "����ʱ��: " << (*t)->getServeTime() << endl;
				}

				cout <<"timer: "<<timer<< " ������: " << (*it)->getName() << " ʵ������ʱ��: " << (*it)->getActualServeTime() <<"��ɼ���: "<<finishCount<< endl;
				cout << "������С: " << v->size() << endl;
				if (finishCount == v->size()) {
					break;
				}
				 if ((*it)->getStartTime() <= timer){
					it = v->begin();
					(*it)->setTurnOverTime(timer - (*it)->getStartTime());
					(*it)->setActualServeTime((*it)->getActualServeTime() + piece);
					if ((*++it)->getStartTime() > timer){
						it--;
						cout << "������2: " << (*it)->getName() << "����ʱ��: " << (*it)->getServeTime() << endl;

						if ((*it)->getActualServeTime() == (*it)->getServeTime()) {
							cout << "������3: " << (*it)->getName() << endl;
							cout << "ʵ������ʱ��: " << (*it)->getActualServeTime() << " ����ʱ��2: " << (*it)->getServeTime() << endl;

							(*it)->setFinishTime(timer + piece);
							finishCount++;
							v->push_back(*it);			/*����һԪ�طŵ�����β��*/
							v->erase(v->begin());

							it = v->begin();
						}
					}
					else if ((*++it)->getStartTime() <= timer) {
						it = v->begin();
						cout << "here" <<(*it)->getName()<< endl;
						if ((*it)->getActualServeTime() == (*it)->getServeTime()) {
							(*it)->setFinishTime(timer + piece);
							finishCount++;
						}
						if ((*++it)->getFinishTime() == 0) {
							cout << "����ʱ��Ϊ0? " << (*it)->getFinishTime() << endl;
							it--;
							v->push_back(*it);			/*����һԪ�طŵ�����β��*/

							v->erase(v->begin());

							it = v->begin();
						}
						else {
							it--;
						}
					}
 				}
				timer++;
			}

			int avTurnOverTime = 0;
//			int avWeightTurnOverTime = 0;
			for (vector<PCB*>::iterator it = v->begin(); it != v->end(); it++) {
				avTurnOverTime += (*it)->getTurnoverTime();
//				avWeightTurnOverTime += (*it)->getTurnoverTime();
				cout << "������: " << (*it)->getName() << "\t" << (*it)->getStartTime() << "\t" << (*it)->getServeTime() << "\t" << (*it)->getFinishTime() << "\t" << (*it)->getTurnoverTime()
					<< endl;
			}
			cout << "ƽ����תʱ��Ϊ: " << avTurnOverTime / v->size() << endl;
//			cout << "��Ȩƽ����תʱ��Ϊ: " << avWeightTurnOverTime / v->size() << endl;
		}
		else if (order == "ls") {
			system->getFileSystem()->TraversalDirectory(system->getFileSystem()->currentFile->sub);
			cout << endl;

		}
		else if (order == "fs") {
			system->getFileSystem()->Traversal(system->getFileSystem()->root);
			cout << endl;

		}
		else if (order.substr(0, 2) == "cd"&&order.length()>2) {
			string path = order.substr(2, order.length()-2);
//			cout << path << endl;
			if (!system->getFileSystem()->AccessDirectory(path)) {
				cerr << "Ŀ¼������" << endl;
			}
		}
		else if (order.substr(0, 3) == "cat") {
			string path = order.substr(3, order.length() - 3);
			vector<string> v = system->getFileSystem()->readFile(path);
			for (vector<string>::iterator it = v.begin(); it != v.end(); it++) {
				cout << *it << endl;
			}
			cout << endl;

		}
		else if (order.substr(0, 4) == "file"){
			string path = order.substr(4, order.length() - 4);
			if (!system->getFileSystem()->makeFile(path)) {
				cout << "�����ļ�ʧ��" << endl;
			}
		}
		else if (order.substr(0, 4) == "mkdr"){
			string path = order.substr(4, order.length() - 4);
			if (!system->getFileSystem()->makeDirectory(path)) {
				cout << "�����ļ���ʧ��" << endl;
			}
		}
		else if (order == "cd") {
			system->getFileSystem()->returnDirectory();
		}
		else if (order == "help") {
			cout << "system\t�鿴ϵͳ״̬\t\tmemory\t�鿴�ڴ�״̬" << endl;
			cout << "md\t�鿴�ڴ���ϸ״̬\t\tfcfs\tģ�������ȷ�������㷨" << endl;
			cout << "timepiece\tģ��ʱ��Ƭ��ת\t\tshutdown\t�ر�ϵͳ" << endl;
			cout << "ls\t�鿴��ǰ�ļ���\t\tcat\t�鿴�ļ�����" << endl;
			cout << "cd\t�����ļ���\t\tfs\t�����ļ�ϵͳ" << endl;
			cout << "mkdr\t�����ļ���\t\tfile\t�����ļ�" << endl;
			cout << endl;
		}
 	}
}


SystemPCB::SystemPCB(int id, std::string name, int size,System*& system):PCB(id,name,size,system){
	setPriority(10);
//	costMemory(system->getMemory());
}

UserPCB::UserPCB(int id, string name, int size, System*& system) : PCB(id, name, size, system){
	setPriority(7);
}

FileSystemPCB::FileSystemPCB(int id, string name, int size, System*& system) : PCB(id, name, size, system) {
	setPriority(8);
}

bool sortByStartTime(PCB* p1, PCB* p2) {
	return p1->getStartTime() < p2->getStartTime();
}