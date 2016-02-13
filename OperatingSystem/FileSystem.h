#include "stdafx.h"
#include <vector>

using namespace std;

/*�ļ���Ŀ¼���*/
class File{
public:
	File *next;			//��һ���ļ����
	File *sub;				//�ӽڵ�
	File *father;			//���ڵ�
	File(string,bool);

	bool isFileDirectory();
	string getName();
private:
	string name;
	bool isDirectory;
};

class FileSystem {
public:
	FileSystem();
	File *root;		//�ļ����ڵ�
	File *currentFile;	//��ǰ���

	void Traversal(File *);
	void TraversalDirectory(File*);
	bool AccessDirectory(string);
	vector<string> readFile(string);

	bool makeFile(string);
	bool makeDirectory(string);
	bool returnDirectory();
//	bool deleteFile(string);
private:
	void FileSystemInit();
};