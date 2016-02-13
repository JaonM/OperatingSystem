#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "stdafx.h"
#include "System.h"
#include "tinyxml.h"
#include <exception>

using namespace std;

string WORD2string(WORD w)
{
	char tmpbuff[16];

	sprintf_s(tmpbuff, "%d", w);

	string res = tmpbuff;

	return res;

}
class XMLParser{
public:
	static boolean parseConfiguration(char* fileName,System*& system) {
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		string time = WORD2string(st.wYear)+"年"+WORD2string(st.wMonth)+"月"+WORD2string(st.wDay)+"日"+" "+WORD2string(st.wHour)+"时"+
			WORD2string(st.wMinute)+"分"+WORD2string(st.wSecond)+"秒";
//		System* system=nullptr;
		try {
			TiXmlDocument doc;
			if (!doc.LoadFile(fileName)) {
				std::cerr << "cannot load file" << std::endl;
				return false;
			}
			TiXmlElement* root = doc.FirstChildElement();
			if (root == nullptr) {
				std::cerr << "there is no root element" << std::endl;
				doc.Clear();
				return false;
			}
			for (TiXmlElement* elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {
				/*std::cout << elem->Value() << std::endl;
				if (strcmp(elem->Value(), "Users") == 0) {
					cout << "here" << endl;
					TiXmlElement* e1 = elem->FirstChildElement();
					std::cout << e1->Value() << std::endl;
					TiXmlNode* node = e1->FirstChild();
					cout << "node " << node->Value() << endl;
					cout << node->ToElement()->GetText()<< endl;
				}*/
				string elementName = elem->Value();
				if (elementName == "Version") {
					string version = elem->GetText();
		//			system.setVersion(version);
//					cout << "versioni: " << version << endl;
					cout << "加载系统版本.." << endl;
					system->setVersion(version);
				}
				else if (elementName == "Users") {
					for (TiXmlElement* childElem = elem->FirstChildElement("User"); childElem != nullptr; childElem = childElem->NextSiblingElement("User")) {
						const char* id = childElem->Attribute("id");
						const char* name = childElem->Attribute("name");
						//cout << "id: " << id << " " << "name: " << name << endl;
						TiXmlNode* node = childElem->FirstChild();
						const char* account = node->ToElement()->GetText();
	//					cout << "account: " << node->ToElement()->GetText() << endl;
						node = childElem->IterateChildren(node);
						const char* password = node->ToElement()->GetText();
//						cout << "password: " << node->ToElement()->GetText() << endl;
						User user(atoi(id), name);
						user.setAccount(account);
						user.setPassword(password);
						system->getUserQueue()->push_back(user);
					}

				}
				else if (elementName == "Memory") {
					TiXmlNode* childElement = elem->FirstChild();
					int memorySize = atoi(childElement->ToElement()->GetText());
					cout << "加载内存中.."<< endl;
					cout << memorySize<<endl;
					childElement=childElement->NextSibling();
		//			Memory memory(memorySize);
					/*childElement = childElement->IterateChildren(childElement);*/
					string strategy = childElement->ToElement()->GetText();
					system = new System(time, memorySize);
					system->setStrategy(strategy);
				}
			}
			doc.Clear();
		}
		catch (const exception& e) {
			return false;
		}

		return true;
	}
};

#endif