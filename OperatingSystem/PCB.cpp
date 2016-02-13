#include "stdafx.h"
#include "PCB.h"
#include <time.h>
#include <map>
#include <cmath>

#define random(x){rand()%x}

using namespace std;

/*PCB类*/
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
	if (strategy == "FIRST_FIT") {		/*首次适应算法*/
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
			cout << "分配内存失败" << endl;
		}
	}
	else if (strategy == "BEST_FIT") {			/*最佳匹配算法*/
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
				cout << "内存分配失败" << endl;
			}
		}
		else {
			cout << " 内存分配失败" << endl;
		}
	}
}

/*ConsolePCB类*/
//根据内存分配算法分配内存		首次适应内存分配算法
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
	//	cout <<"开始时间: "<< getStartTime() << endl;
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
			cout << "进程调度 先来先服务模拟" << endl;
			vector<PCB*>* v = system->getProcessQueue();
			sort(v->begin(), v->end(), sortByStartTime);
			int avTurnOverTime = 0;
			int avWeightTurnOverTime = 0;
			for (vector<PCB*>::iterator it = v->begin(); it != v->end(); it++) {
				//cout << (*it)->getName() << " 到达时间: " << (*it)->getStartTime()<<" ";
				avTurnOverTime += (*it)->getTurnoverTime();
				avWeightTurnOverTime += (*it)->getTurnoverTime();
				cout << "进程名: " << (*it)->getName() << "\t" << (*it)->getStartTime() << "\t" << (*it)->getServeTime() << "\t" << (*it)->getFinishTime() << "\t" << (*it)->getTurnoverTime()
					<< "\t" << (*it)->getWeightTurnoverTime() << endl;
			}
			 
			cout << "平均周转时间为: " << avTurnOverTime / v->size() << endl;
			cout << "带权平均周转时间为: " << avWeightTurnOverTime / v->size() << endl; 
			cout << endl;
		}
		else if (order == "timepiece") {/*时间片轮转算法*/
			cout << "输入时间片大小" << endl;
			int piece;
			cin >> piece;
			int timer = 0;		/*开始时间*/
			bool isStop = false;
			vector<PCB*>* v = system->getProcessQueue();
			/*初始化*/
			for (vector<PCB*>::iterator it = v->begin(); it != v->end(); it++){
				(*it)->setFinishTime(0);
				(*it)->setTurnOverTime(0);
			}
			sort(v->begin(), v->end(), sortByStartTime);

			vector<PCB*>::iterator it = v->begin();
			for (vector<PCB*>::iterator t = it; t != v->end(); t++) {
				cout << "进程名: " << (*t)->getName() << "服务时间: " << (*t)->getServeTime() << endl;
			}
			int finishCount = 0;
			while (!isStop) {
				for (vector<PCB*>::iterator t = it; t != v->end(); t++) {
					cout << "进程名: " << (*t)->getName() << "服务时间: " << (*t)->getServeTime() << endl;
				}

				cout <<"timer: "<<timer<< " 进程名: " << (*it)->getName() << " 实际运行时间: " << (*it)->getActualServeTime() <<"完成计数: "<<finishCount<< endl;
				cout << "向量大小: " << v->size() << endl;
				if (finishCount == v->size()) {
					break;
				}
				 if ((*it)->getStartTime() <= timer){
					it = v->begin();
					(*it)->setTurnOverTime(timer - (*it)->getStartTime());
					(*it)->setActualServeTime((*it)->getActualServeTime() + piece);
					if ((*++it)->getStartTime() > timer){
						it--;
						cout << "进程名2: " << (*it)->getName() << "服务时间: " << (*it)->getServeTime() << endl;

						if ((*it)->getActualServeTime() == (*it)->getServeTime()) {
							cout << "进程名3: " << (*it)->getName() << endl;
							cout << "实际运行时间: " << (*it)->getActualServeTime() << " 服务时间2: " << (*it)->getServeTime() << endl;

							(*it)->setFinishTime(timer + piece);
							finishCount++;
							v->push_back(*it);			/*将第一元素放到队列尾部*/
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
							cout << "结束时间为0? " << (*it)->getFinishTime() << endl;
							it--;
							v->push_back(*it);			/*将第一元素放到队列尾部*/

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
				cout << "进程名: " << (*it)->getName() << "\t" << (*it)->getStartTime() << "\t" << (*it)->getServeTime() << "\t" << (*it)->getFinishTime() << "\t" << (*it)->getTurnoverTime()
					<< endl;
			}
			cout << "平均周转时间为: " << avTurnOverTime / v->size() << endl;
//			cout << "带权平均周转时间为: " << avWeightTurnOverTime / v->size() << endl;
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
				cerr << "目录不存在" << endl;
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
				cout << "创建文件失败" << endl;
			}
		}
		else if (order.substr(0, 4) == "mkdr"){
			string path = order.substr(4, order.length() - 4);
			if (!system->getFileSystem()->makeDirectory(path)) {
				cout << "创建文件夹失败" << endl;
			}
		}
		else if (order == "cd") {
			system->getFileSystem()->returnDirectory();
		}
		else if (order == "help") {
			cout << "system\t查看系统状态\t\tmemory\t查看内存状态" << endl;
			cout << "md\t查看内存详细状态\t\tfcfs\t模拟先来先服务调度算法" << endl;
			cout << "timepiece\t模拟时间片轮转\t\tshutdown\t关闭系统" << endl;
			cout << "ls\t查看当前文件夹\t\tcat\t查看文件内容" << endl;
			cout << "cd\t访问文件夹\t\tfs\t遍历文件系统" << endl;
			cout << "mkdr\t创建文件夹\t\tfile\t创建文件" << endl;
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