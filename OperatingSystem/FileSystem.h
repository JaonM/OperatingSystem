#include "stdafx.h"
#include <vector>

using namespace std;

/*文件或目录结点*/
class File{
public:
	File *next;			//下一个文件结点
	File *sub;				//子节点
	File *father;			//父节点
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
	File *root;		//文件根节点
	File *currentFile;	//当前结点

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