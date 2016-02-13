#include "stdafx.h"
#include "FileSystem.h"
#include <iostream>
#include <fstream>

using namespace std;

File::File(string name, bool isDirectory) :name(name), isDirectory(isDirectory){}

bool File::isFileDirectory(){
	return isDirectory;
}

string File::getName() {
	return name;
}

void FileSystemInit();
FileSystem::FileSystem() {
	FileSystemInit();
}


void FileSystem::Traversal(File *root) {
	if (root != nullptr) {
		for (File* file = root; file != nullptr; file = file->next){
			cout << file->getName() << " ";
			Traversal(file->sub);
		}
		cout << "\n\r";
	}
	else {
		return;
	}
}
void FileSystem::TraversalDirectory(File *root){
	for (File* file = root; file != nullptr; file = file->next) {
		cout << file->getName() << "\t";
	}
	cout << endl;
}
bool FileSystem::AccessDirectory(string fileName) {
//	cout << "当前文件: "<<currentFile->getName()<< endl;

	bool isExist = false;
	for (File* temp = currentFile->sub; temp != nullptr; temp = temp->next) {
		if (temp->getName() == fileName&&temp->isFileDirectory()){
			isExist = true;
			currentFile = temp;
			break;
		}
	}
	if(!isExist) {
		cerr << "文件夹不存在" << endl;
	}
	return true;
}
vector<string> FileSystem::readFile(string path) {
	vector < string > v;
	ifstream ifs(path);
	string temp;
	while (getline(ifs, temp)) {
		v.push_back(temp);
	}
	return v;
}
bool FileSystem::makeFile(string path) {
	ofstream osf(path);
	if (osf.is_open()) {
		File* file = currentFile->sub;
		if (file == nullptr) {
			file = currentFile->sub = new File(path, "false");
			file->next = nullptr;
			file->father = currentFile;
			file->sub = nullptr;
		}
		else {
			while (file->next != nullptr) {
				file = file->next;
			}
			file->next = new File(path, "false");
			file = file->next;
			file->father = currentFile;
			file->sub = nullptr;
		}
		
		return true;
	}
	else {
		return false;
	}
}

bool FileSystem::returnDirectory() {
	if (currentFile->father != nullptr) {
		currentFile = currentFile->father;
	}
	return true;
}

bool FileSystem::makeDirectory(string path) {
	ofstream osf(path);
	if (osf.is_open()) {
		File* file = currentFile->sub;
		while (file->next != nullptr) {
			file = file->next;
		}
		file->next = new File(path, "true");
		file = file->next;
		file->father = currentFile;
		return true;
	}
	else {
		return false;
	}
}
void FileSystem::FileSystemInit() {
	currentFile = root = new File("~", true);
	root->father = nullptr;
	root->next = nullptr;
	
	/*init system*/
	File* system=root->sub = new File("System", true);
	system->father = root;
	File* temp = system->sub = new File("TEMP", true);
	temp->father = system;
	File* tempSub = temp->sub = new File("a.a", false);
	tempSub->father = temp;
	tempSub->sub = nullptr;
	tempSub->next = new File("b.b", false);
	tempSub = tempSub->next;
	tempSub->father = temp;
	tempSub->sub = nullptr;
	tempSub->next = new File("c.c", false);
	tempSub = tempSub->next;
	tempSub->father = temp;
	tempSub->sub = nullptr;
	tempSub->next = nullptr;

	temp->next = new File("Recycle", "true");
	temp = temp->next;
	temp->father = system;
	temp->sub = nullptr;
	temp->next = nullptr;

	/*init User*/
	system->next = new File("User", true);
	system = system->next;
	system->father = root;
	File* root = system->sub = new File("Root.txt", false);
	root->father = system;
	root->sub = nullptr;
	root->next = new File("Temp.txt", false);
	root = root->next;
	root->father = system;
	root->next = nullptr;
	root->sub = nullptr;

	/*init Configuration.xml*/
	system->next = new File("Configuration.xml", false);
	system = system->next;
	system->sub = nullptr;
	system->father = root;

	/*init readme.txt*/
	system->next = new File("readme.txt", false);
	system = system->next;
	system->father = root;
	system->next = nullptr;
	system->sub = nullptr;
	ofstream of;
	of.open(system->getName());
	if (of.is_open()) {
		char* str = "readme create by ma";
		of.write(str,strlen(str));
	}
	else {
		cerr << "Configuration.xml创建失败" << endl;
	}
	of.close();


}