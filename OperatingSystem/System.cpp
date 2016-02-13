
#include"stdafx.h"
#include "System.h"

#define M_BLOCK_NUM 10	//定义内存块结构10
#define random(x) (rand()%x)	//产生0~x随机数

/*计时器类*/
void Time::delay(int time)  {
	clock_t now = clock();
	while (clock() - now < time);
}

/*MemoryBlock类*/
int MemoryBlock::getTag() {
	return this->tag;
}
void MemoryBlock::setTag(int tag){
	this->tag = tag;
}
int MemoryBlock::getSize(){
	return this->size;
}
int MemoryBlock::getId(){
	return this->id;
}
MemoryBlock* MemoryBlock::FootLocate(MemoryBlock* p) {
	return p + p->size;
}
void MemoryBlock::setFree(int free){
	this->free = free;
}
int MemoryBlock::getFree(){
	return size-getNonFree();
}
void MemoryBlock::setNonFree(int nonFree){
	this->nonfree = nonFree;
}
int MemoryBlock::getNonFree(){
	return nonfree;
}
MemoryBlock::MemoryBlock(int id,int tag, int size) :id(id),tag(tag), size(size){
//	FootLocate(this)->tag = 0;
//	FootLocate(this)->up = this;
	setFree(size);
	setNonFree(0);
	up=this;
}
void MemoryBlock::setProcessName(string processName){
	this->processName = processName;
}
string MemoryBlock::getProcessName(){
	return processName;
}

/*内存类*/
Memory::Memory(int size) :size(size) {
	currentMemory = 0;		//初始化占用的大小
	
	/*初始化内存块链表结构*/
	this->head = new MemoryBlock(0,0, 0);	//头结点值为空
	head->left = nullptr;
	int blockSize=random(size);
	int leftMemorySize = size - blockSize;
	MemoryBlock *p = head;
	int id = 1;
	for (int i = 1; i <=M_BLOCK_NUM; i++) {
		if (blockSize != 0) {
			p->right = new MemoryBlock(id, 0, blockSize);
			p->right->left = p;
			p->right->right = nullptr;
			p = p->right;
			id++;
		}
		
		if (i == M_BLOCK_NUM - 1) {
			blockSize = leftMemorySize;
		}
		else{
			blockSize = random(leftMemorySize);
			leftMemorySize = leftMemorySize - blockSize;
		}
//		cout << "blockSize: " << blockSize << endl;
//		leftMemorySize = leftMemorySize - blockSize;
//		p->right= new MemoryBlock(0, blockSize);
////		p->right = t;
//		p->right->left = p;
//		p->right->right = nullptr;
//		p = p->right;
//		cout <<i<<" now: "<<p->getSize() << endl;
//		cout << i<<" prev: " << p->left->getSize() << endl;

	}
}

Memory::Memory(const Memory& src):size(src.size) {
	currentMemory = sizeof(this);		//初始化占用的大小

	/*初始化内存块链表结构*/
	this->head = new MemoryBlock(0, 0, 0);	//头结点值为空
	head->left = nullptr;
	int blockSize = random(size);
	int leftMemorySize = size - blockSize;
	MemoryBlock *p = head;
	int id = 1;
	for (int i = 1; i <= M_BLOCK_NUM; i++) {
		if (blockSize != 0) {
			p->right = new MemoryBlock(id, 0, blockSize);
			p->right->left = p;
			p->right->right = nullptr;
			p = p->right;
			id++;
		}

		if (i == M_BLOCK_NUM - 1) {
			blockSize = leftMemorySize;
		}
		else{
			blockSize = random(leftMemorySize);
			leftMemorySize = leftMemorySize - blockSize;
		}
	}
}
//Memory::~Memory() {
//	delete this;
//}
int Memory::getCurrentMemory() {
	currentMemory = 0;
	for (MemoryBlock* p = getAddress(); p != nullptr; p = p->right) {
		if (p->getSize() != 0){
			currentMemory += p->getNonFree();
		}
	}
	return this->currentMemory;
}
void Memory::setCurrentMemory(int value) {
	this->currentMemory = value;
}
int Memory::getLeftMemory() {
	return this->size - this->getCurrentMemory();
}
boolean Memory::isOutOfMemory() {
	return this->currentMemory>this->size;
}
int Memory::getSize() {
	cout << "size: " << size<<endl;
	return this->size;
}
MemoryBlock* Memory::getAddress(){
	return this->head;
}
void Memory::showMemory(){
	cout << "总内存: " << size << "KB" << "已用内存: " << currentMemory << "KB" << "剩余内存: " << getLeftMemory() << endl;
}
void Memory::showMemoryDetail(){
	MemoryBlock* p = this->head;
	showMemory();
	cout << "各内存块详细情况如下:" << endl;
	while (p != nullptr) {
		cout << "id: " << p->getId() << " 内存块大小为: " << p->getSize() << "KB 内存地址: " << p <<"内存状态: "<<p->getTag()<<  "已用内存: "<<p->getNonFree()<<"KB "<<"剩余内存: "<<p->getFree()<<"KB"<<
			" 占用进程名: "<<p->getProcessName()<<endl;
		p = p->right;
	}
}



/*System类*/
System::System(string sysTime,int memorySize) :sysTime(sysTime){
	memory = new Memory(memorySize);
	processQueue = new vector<PCB*>();
	userQueue = new vector<User>();
	fileSystem = new FileSystem();
}
void System::setVersion(string version) {
	this->version = version;
}
string System::getVersion(){
	return this->version;
}
vector<PCB*>* System::getProcessQueue(){
	return this->processQueue;
}
vector<User>* System::getUserQueue(){
	return this->userQueue;
}
//void System::setMemory(Memory memory){
//	this->memory=memory;
//}
Memory* System::getMemory(){
	return this->memory;
}
void System::setSysTime(string sysTime){
	this->sysTime = sysTime;
}
string System::getSysTime(){
	return this->sysTime;
}
string System::getCurrentUser(){
	return this->currentUser;
}
//System::~System(){
//	int processSize = processQueue.size();
//	for (int i = 0; i < processSize; i++) {
//		PCB* p = processQueue.front();
//		delete p;
//		processQueue.pop();
//	}
//	/*int userSize = userQueue.size();
//	for (int i = 0; i < userSize; i++) {
//		User u = userQueue.front();
//		delete &u;
//		userQueue.pop();
//	}*/
//	delete &memory;
//}
FileSystem* System::getFileSystem() {
	return fileSystem;
}

void System::setCurrentUser(string user){
	this->currentUser = user;
}
void System::setStrategy(string strategy){
	this->memoryStrategy = strategy;
}
string System::getStrategy(){
	return this->memoryStrategy;
}
void System::output(){
	cout << "系统当前时间: "<<this->getSysTime()<<endl;
	cout << "系统内存总量: " << this->getMemory()->getSize() << "KB" <<
		" 已使用: " << getMemory()->getCurrentMemory() << "KB" << " 剩余: " << getMemory()->getLeftMemory() << endl;
	cout << "系统用户: " << endl;
	/*int userSize = userQueue->size();
	for (int i = 0; i < userSize; i++){
		User user = userQueue->front();
		cout << user.getName() << " ";
		userQueue->pop();
	}*/
	for (vector<User>::iterator it = userQueue->begin(); it != userQueue->end(); it++) {
		cout << it->getName() << " ";
	}
	cout << "\r\n" ;
	cout << "系统当前使用用户: " << currentUser<<endl;
	int pSize = processQueue->size();
	cout <<"系统当前进程数量: "<<pSize<< " 系统当前进程名: " << endl;

	for (vector<PCB*>::iterator it = processQueue->begin(); it != processQueue->end(); it++) {
		cout << (*it)->getName() << " ";
	}
	cout << endl;
}