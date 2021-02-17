#pragma once
#include <iostream>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <cstdarg>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sys/types.h>
#include <direct.h>
#include <sstream>



using namespace boost::algorithm;
using namespace std;

enum class AttTypes
{
	Str = 0,
	Int,
	Flt
};

class Attribute
{
	string name;
	string type;
	int dim;
	string valImpl;
public:
	Attribute()
	{

	};
	Attribute(string nume, AttTypes type, int dim, string valImpl)
	{
		this->name = nume;
		this->dim = dim;
		this->valImpl = valImpl;
		switch (type) {
		case AttTypes::Str:
		{
			this->type = "string";
			break;
		}
		case AttTypes::Int: {
			this->type = "int";
			break;
		}
		case AttTypes::Flt: {
			this->type = "float";
			break;
		}
		}
	}
	void setName(string name)
	{
		this->name = name;
	}
	void setType(string type)
	{
		this->type = type;
	}
	void setValImpl(string valImpl)
	{
		this->valImpl = valImpl;
	}
	void setDim(int dim)
	{
		this->dim = dim;
	}
	string getName()
	{
		return this->name;
	}
	string getType()
	{
		return this->type;
	}
	string getValImpl()
	{
		return this->valImpl;
	}
	int getDim()
	{
		return this->dim;
	}
};

class Register
{
	int nrCol;
	vector<string> vals;


public:
	Register() {};
	Register(vector<string> date)
	{
		nrCol = date.size();
		vals = date;
		
	}
	~Register()
	{
		
	}
	void setVals(string* vals) {
		for (int i = 0;i < nrCol;i++)
			this->vals[i] = vals[i];
	}
	string getValue(int i)
	{
		if (i >= 0 && i < nrCol)
			return this->vals[i];
	}
	vector<string> getValues()
	{
		return vals;
	}

	void write(std::fstream& f)
	{
		size_t size;
		size_t dim;

		dim = vals.size();
		f.seekg(0, std::ios::end);
		f.write((char*)&dim, sizeof(size_t));
		for (size_t i = 0; i < dim; i++)
		{
			size = vals[i].size();
			f.write((char*)&size, sizeof(size_t));
			f.write((char*)vals[i].c_str(), size);
		}
		
	}
	void read(std::fstream& f)
	{
		size_t dim;
		size_t size;
		char* data;


		f.read((char*)&dim, sizeof(size_t));
		for (size_t i = 0; i < dim; i++)
		{
			f.read((char*)&size, sizeof(size_t));
			data = new char[size + 1];
			f.read(data, size);
			data[size] = '\0';
			vals.push_back(data);
			delete data;
		}
		nrCol = dim;
	}
};

class Table
{
	Attribute* a;
	std::vector<Register*> r;
	int nrCol;
	string TableName;
public:
	Table() {};
	Table(string TableName, Attribute* attribs, int nrAttribs)
	{
		this->TableName = TableName;
		a = new Attribute[nrAttribs];
		nrCol = nrAttribs;
		
		for (int i = 0; i < nrCol; i++)
		{
			a[i] = attribs[i];
		}
	}
	~Table()
	{
		r.clear();
	}
	void addRegister(Register* reg) {
		
		r.push_back(reg);
		
	}
	
	int deleteRegister(string value, string nume_col)
	{
		int copy1 = -1;
		int copy2 = -1;                                                     
		for (int j = 0;j < nrCol;j++)
		{
			if (a[j].getName() == nume_col)
			{
				copy1 = j;

			}
		}
		if (copy1 == -1)
		{
			cout << "\nNu exista coloana cu numele " << nume_col;
			return 0;
		}
		for (int i = 0;i < r.size();i++)
		{
			if (r[i]->getValue(copy1) == value)
				copy2 = i;
		}
		if (copy2 == -1)
		{
			cout << "\nNu exista astfel de inregistrare ";
			return 0;
		}
		Register* aux;
		int dim = r.size();
		aux = r[copy2];
		r[copy2] = r[dim - 1];
		r[dim - 1] = aux;
		r.pop_back();

		return 1;
	}
	void selectFromTable()
	{
		cout << setw(20) << "Table: " << TableName << endl;
		for (int i = 0;i < nrCol;i++)
			cout << setw(20) << a[i].getName() << " ";
		cout << endl;
		
		for (int j = 0;j < r.size();j++)
		{
			for (int k = 0;k < nrCol;k++)
				cout << setw(20) << r[j]->getValue(k) << " ";
			cout << endl;
		}

	}
	void setNrCol(int nrCol)
	{
		this->nrCol = nrCol;
	}

	void setTableName(string tableName)
	{
		this->TableName = tableName;
	}
	int getNrCol()
	{
		return this->nrCol;
	}
	int getNrReg()
	{
		return r.size();
	}
	string getTableName()
	{
		return this->TableName;
	}
	Attribute* getAttributes()
	{
		return this->a;
	}
	vector<Register*> getRegisters()
	{
		return this->r;
	}
};

class DataBase
{
	Table** t;
	string nume;
	int nrTab = 0;

public:
	~DataBase()
	{
		delete t;
	}

	DataBase(string nume)
	{
		t = new Table * [nrTab];
		for (int i = 0;i < nrTab;i++)
			t[i] = new Table();
		this->nume = nume;
	}

	void createTable(string numeT, Attribute* attrbs, int nrAttribs)
	{
		t[nrTab] = new Table(numeT, attrbs, nrAttribs);
		nrTab++;
	}

	int getNrTab() {
		return this->nrTab;
	}

	Table* getTableByName(string name)
	{
		for (int i = 0;i < nrTab;i++)
		{
			if (t[i]->getTableName() == name)
				return t[i];

		}
		return nullptr;
	}

	void dropTable(string numeT)
	{
		Table* aux;
		for (int i = 0;i < nrTab;i++)
		{
			if (t[i]->getTableName() == numeT && i < nrTab - 1)
			{
				aux = t[i];
				t[i] = t[nrTab - 1];
				t[nrTab - 1] = aux;
				break;
			}

		}
		delete t[nrTab - 1];
		nrTab--;

		string attrNamer = "Attributes/" + numeT + ".txt";
		remove(attrNamer.c_str());
		string regNamer = "Registers/" + numeT + ".bin";
		remove(regNamer.c_str());
	}
};

class InitDB
{
	DataBase* db;
public:
	InitDB() {
		db = new DataBase("Test");
		const char* pathAttr = "Attributes";
		const char* pathReg = "Registers";
		std::vector<std::string> attrFiles;
		std::vector<std::string> regsFiles;

		if (_mkdir(pathAttr) != 0)
		{
			getDir(pathAttr, attrFiles, "rt");	
			
		}
		if (_mkdir(pathReg) != 0)
		{
			getDir(pathReg, regsFiles, "rb");
			
		}	

		if (!attrFiles.empty())
		{
			for (size_t i = 0; i < attrFiles.size(); i++)
			{
				int dim = attrFiles[i].length();
				string tableName = attrFiles[i].substr(0, dim-5);
				string fileName =  attrFiles[i].substr(0, dim - 1);
				string pathFile = string(pathAttr) + "/"+ fileName;
				std::fstream fis(pathFile, ios::in);
				if (fis)
				{   
					string line;
					int nr = 0;
					while (!fis.eof())
					{
						getline(fis, line);
						nr++;
					}
					Attribute* attributes = new Attribute[nr] ;
					int nr_col = 0;
					string attribute_str;
					fis.clear();
					fis.seekg(0);
					while (std::getline(fis, attribute_str))
					{
						for (auto& c : attribute_str) c = toupper(c);
						std::vector<string> cuvinte;

						int pos = 0;
						//i = 0;
						while ((pos = attribute_str.find(" ")) != std::string::npos) {
							string token = attribute_str.substr(0, pos);
							if (token != "")
							{
								cuvinte.push_back(token);
							}
							attribute_str.erase(0, pos + 1);
						}
						cuvinte.push_back(attribute_str);

						AttTypes tip;
						if (cuvinte[1] == "STRING")
							tip = AttTypes::Str;
						else if (cuvinte[1] == "INT")
							tip = AttTypes::Int;
						else if (cuvinte[1] == "FLOAT")
							tip = AttTypes::Flt;
						else
						{
							cout << "\ntip argument incorect";
						}

						Attribute* attribute = new Attribute(cuvinte[0], tip, std::stoi(cuvinte[2]), cuvinte[3]);
						attributes[nr_col] = *attribute;
						nr_col++;					
					}				
					db->createTable(tableName, attributes, nr_col);

				}
				fis.close();
			}
		}

		if (!regsFiles.empty())
		{
			for (size_t i = 0; i < regsFiles.size(); i++)
			{
				int dim = regsFiles[i].length();
				string tableName = regsFiles[i].substr(0, dim - 6);
				string fileName = regsFiles[i].substr(0, dim - 2);
				string pathFile = string(pathReg) + "/" + fileName;
				std::fstream fis(pathFile, ios::in|ios::binary);
				fis.seekg(0, std::ios::end);
				if(fis && fis.tellg() != 0)
				{
					fis.clear();
					fis.seekg(0);
					Table* tb = db->getTableByName(tableName);
					while (!fis.eof())
					{
						try {
							Register* r = new Register();
							r->read(fis);
							if (r->getValues().size() != 0)
								tb->addRegister(r);
						}
						catch (exception e) {
							int i = 1;

						}
					}
				}
				fis.close();
			}
		}

	}

	DataBase* GetDatabase()
	{
		return db;
	}

	void getDir(string d, vector<string>& f, char const* mode)
	{
		FILE* pipe = nullptr;
		string pCmd = "dir /B " + d;
		char buf[256];
		pipe = _popen(pCmd.c_str(), mode);
		if (nullptr == pipe)
		{
			return;
		}
		while (!feof(pipe))
		{
			if (fgets(buf, 256, pipe) != nullptr)
			{
				f.push_back(string(buf));
			}
		}
		_pclose(pipe);
	}
};

static class DBCommands
{
public:

	static int CreateTable(DataBase* db, string tableName, vector<string>args)
	{

		if (db->getTableByName(tableName) != nullptr)
		{
			cout << "\nTabela cu numele " << tableName << " exista deja";
			return 0;
		}
		if (args.size() % 4 != 0)
		{
			cout << "\nNr de parametri introdus incorect";
			return 0;
		}

		Attribute* listaAtribute = new Attribute[args.size() / 4]{};

		string nume;
		AttTypes tip;
		int dim;
		string valImpl;

		for (int i = 0;i < args.size();i++)
		{
			if (i % 4 == 0)
				nume = args[i];
			if (i % 4 == 1)
			{
				if (args[i] == "STRING")
					tip = AttTypes::Str;
				else if (args[i] == "INT")
					tip = AttTypes::Int;
				else if (args[i] == "FLOAT")
					tip = AttTypes::Flt;
				else
				{
					cout << "\ntip argument incorect";
					return 0;
				}
			}
			if (i % 4 == 2)
			{
				try
				{
					dim = std::stoi(args[i]);
				}
				catch (exception e)
				{
					cout << "\ntip argument incorect";
					return 0;
				}

			}
			if (i % 4 == 3)
			{
				valImpl = args[i];
				listaAtribute[(i - 3) / 4] = Attribute(nume, tip, dim, valImpl);
			}
		}
		string numeFis ="Registers/" + tableName + ".bin";
		std::fstream fis(numeFis, ios::out | ios::binary);
		fis.close();
		string numeFisier ="Attributes/" + tableName + ".txt";
		std::ofstream fisOut(numeFisier, ios::out);
		for (int i = 0;i < args.size() / 4;i++)
			fisOut << listaAtribute[i].getName() << " " << listaAtribute[i].getType() << " " << listaAtribute[i].getDim() << " " << listaAtribute[i].getValImpl()<<endl;
		fisOut.close();
		db->createTable(tableName, listaAtribute, args.size() / 4);
		return 1;

	}

	static int Insert(DataBase* db, string tableName, vector<string> valori)
	{
		Table* table = db->getTableByName(tableName);
		if (table == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << " nu exista";
			return 0;
		}
		Attribute* atts = table->getAttributes();
		int nrAtts = table->getNrCol();
		if(valori.size()!=nrAtts)
		{
			cout << "\nNumar de parametri incorect" << endl;
			return 0;
		}
		for (int i = 0;i < nrAtts;i++)
		{
			if (atts[i].getType() == "int")
			{
				try 
				{
					stoi(valori[i]);
				}
				catch (exception e)
				{
					cout << "\nParametru incorect" << endl;
					return 0;
				}
			}
			if (atts[i].getType() == "float")
			{
				try
				{
					stof(valori[i]);
				}
				catch (exception e)
				{
					cout << "\nParametru incorect" << endl;
					return 0;
				}
			}
		}
		Register* r = new Register(valori);
		table->addRegister(r);

		string numeFis = "Registers/"+ tableName + ".bin";
		std::fstream fis(numeFis, ios::out | ios::binary|ios::app);
		if (!fis)
		{
			cout << "\nNu am gasit fisierul asociat tabelei cu numele " << tableName;
		}
		r->write(fis);
		fis.close();
		return 1;
	}

	static int DropTable(DataBase* db, string tableName)
	{
		if (db->getTableByName(tableName) == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << "nu exista";
			return 0;
		}
		db->dropTable(tableName);
		return 1;
	}

	static int DisplayTable(DataBase* db, string tableName)
	{

		Table* table = db->getTableByName(tableName);
		if (table == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << "nu exista";
			return 0;
		}
		int nrColoane = table->getNrCol();
		Attribute* att = table->getAttributes();
		
		cout << setw(20) << tableName << endl;
		
		for (int i = 0;i < nrColoane;i++)
		{
			cout << "|" << setw(20) << internal<< att[i].getName() << "|" << setw(8) << internal << att[i].getType() << "|" << setw(8) << internal << att[i].getDim() << "|" << setw(8) << internal << att[i].getValImpl() << "|";
			cout << endl;
			

		}
		return 1;
	}

	static int DeleteFromTable(DataBase* db, string tableName, string value, string nume_col)
	{

		Table* table = db->getTableByName(tableName);
		if (table == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << "nu exista";
			return 0;
		}
		int rez =  table->deleteRegister(value, nume_col);


		string numeFis = "Registers/" + tableName + ".bin";

		std::fstream fis(numeFis, ios::out | ios::binary | std::ios::trunc);
		if (!fis)
		{
			cout << "\nNu am gasit fisierul asociat tabelei cu numele " << tableName;
		}

		vector<Register*> reg = table->getRegisters();
		for (size_t i = 0; i < reg.size(); i++)
		{
			reg[i]->write(fis);
		}

		return rez;
	}
	
	static int SelectFromTable(DataBase* db, string tableName)
	{
		Table* table = db->getTableByName(tableName);
		if (table == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << "nu exista";
			return 0;
		}
		else
		{
			table->selectFromTable();
			return 1;
		}
	}

	
	static int ImportTable(DataBase* db, string tableName, string CSVFile)
	{
		const char COMMA = ',';
		const char WHITESPACE = ' ';
		std::string linia;

		if (db->getTableByName(tableName) == nullptr)
		{
			cout << "\nTabela cu numele " << tableName << "nu exista";
			return 0;
		}
		Table* table = db->getTableByName(tableName);
		std::ifstream myFile; 
		myFile.open(CSVFile); 
		if (!myFile) 
		{			
			exit(EXIT_FAILURE);
		}
		while (myFile.peek() != EOF)
		{
			getline(myFile, linia);

			size_t pos = 0;
			std::string token;
			std::string delimiter = ",";
			vector<string> valori;

			while ((pos = linia.find(delimiter)) != std::string::npos) {
				token = linia.substr(0, pos);
				if (token != "")
				{
					valori.push_back(token);
				}
				linia.erase(0, pos + delimiter.length());
			}

			if (pos == std::string::npos && linia != "")
				valori.push_back(linia);

			int rez = Insert(db, tableName, valori);
			if (rez == 0)
			{
				cout << "\nInserarea nu s-a putut realiza";
				return 0;
			}	
		}
		return 1;
	}
};

void insertIntoTable(DataBase* db, string comanda) 
{
	std::string delimiter = " ";
	std::string paranteza1 = "(";
	std::string paranteza2 = ")";
	std::string virgula = ",";
	std::vector<string> cuvinte;
	std::vector<string> valori;

	size_t pos1 = comanda.find(paranteza1); 
	size_t pos2 = comanda.find(paranteza2); 
	if (pos1 == std::string::npos || pos2 == std::string::npos)
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	string valoriStr = comanda.substr(pos1 + 1, pos2 - pos1 - 1);
	size_t pos = 0;
	std::string token;
	int i = 0;

	while ((pos = valoriStr.find(virgula)) != std::string::npos)
	{
		token = valoriStr.substr(0, pos);
		trim(token);
		if (token != "")
		{
			valori.push_back(token);
		}
		valoriStr.erase(0, pos + virgula.length());
	}
	trim(valoriStr);
	valori.push_back(valoriStr);

	comanda.erase(pos1, pos2);

	pos = 0;
	i = 0;
	while ((pos = comanda.find(delimiter)) != std::string::npos) {
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}

	if (pos == std::string::npos && comanda != "") 
		cuvinte.push_back(comanda);
	if (cuvinte.size() != 4)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "INTO" || cuvinte[3] != "VALUES")
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::Insert(db, cuvinte[2], valori);
	if (rez == 1)
		cout << "\nInserare realizata cu succes";
	return;
}

void selectFromTable(DataBase* db, string comanda)
{
	std::string delimiter = " ";
	std::vector<string> cuvinte;

	size_t pos = 0;
	std::string token;
	int i = 0;

	while ((pos = comanda.find(delimiter)) != std::string::npos)
	{
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}
	
	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);
	if (cuvinte.size() != 4)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "ALL" || cuvinte[2] != "FROM") 
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::SelectFromTable(db, cuvinte[3]);

	return;
}

void deleteFromTable(DataBase* db, string comanda)
{
	std::string delimiter = " ";
	std::vector<string> cuvinte;

	size_t pos = 0;
	std::string token;
	int i = 0;

	while ((pos = comanda.find(delimiter)) != std::string::npos)
	{
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}
	
	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);
	if (cuvinte.size() != 7)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "FROM" || cuvinte[3] != "WHERE" || cuvinte[5] != "=")
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::DeleteFromTable(db, cuvinte[2], cuvinte[6], cuvinte[4]);
	if (rez == 1)
		cout << "\nStergere inregistrare realizata cu succes";
	return;
}

void createTable(DataBase* db, string comanda)
{
	std::string delimiter = " ";
	std::string paranteza1 = "(";
	std::string paranteza2 = ")";        
	std::string virgula = ",";
	std::vector<string> cuvinte;
	std::vector<string> valori;

	size_t pos1 = comanda.find_first_of(paranteza1); 
	size_t pos2 = comanda.find_last_of(paranteza2); 
	if (pos1 == std::string::npos || pos2 == std::string::npos)
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	string valoriStr = comanda.substr(pos1 + 1, pos2 - 1);
	size_t pos = 0;
	std::string token;
	int i = 0;

	while ((pos = valoriStr.find_first_of(virgula)) != std::string::npos) {
		token = valoriStr.substr(0, pos);
		trim(token);
		token.erase(remove(token.begin(), token.end(), '('), token.end());
		token.erase(remove(token.begin(), token.end(), ')'), token.end());
		trim(token);
		if (token != "")
		{
			valori.push_back(token);
		}
		valoriStr.erase(0, pos + virgula.length());
	}
	
	valoriStr.erase(remove(valoriStr.begin(), valoriStr.end(), ')'), valoriStr.end());
	trim(valoriStr);
	if (pos == std::string::npos)
		valori.push_back(valoriStr);

	comanda.erase(pos1, pos2);

	pos = 0;
	i = 0;
	while ((pos = comanda.find(delimiter)) != std::string::npos) {
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}

	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);

	if (cuvinte.size() != 3)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "TABLE")
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::CreateTable(db, cuvinte[2], valori);
	if (rez == 1)
		cout << "\nCrearea tabelei "<<cuvinte[2]<<" s-a realizat cu succes";
	return;
}

void dropTable(DataBase* db, string comanda)
{

	size_t pos = 0;
	std::string token;
	std::string delimiter = " ";
	std::vector<string> cuvinte;

	while ((pos = comanda.find(delimiter)) != std::string::npos) {
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}

	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);

	if (cuvinte.size() != 3)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "TABLE")
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::DropTable(db, cuvinte[2]);
	if (rez == 1)
		cout << "\nStergerea tabelei s-a realizat cu succes";
	return;

}

void displayTable(DataBase* db, string comanda)
{
	size_t pos = 0;
	std::string token;
	std::string delimiter = " ";
	std::vector<string> cuvinte;

	while ((pos = comanda.find(delimiter)) != std::string::npos) {
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}
	
	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);

	if (cuvinte.size() != 3)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	if (cuvinte[1] != "TABLE")
	{
		cout << "\ncomanda incorecta!";
		return;
	}

	int rez = DBCommands::DisplayTable(db, cuvinte[2]);
	return;
}

void importTable(DataBase* db, string comanda)
{
	size_t pos = 0;
	std::string token;
	std::string delimiter = " ";
	std::vector<string> cuvinte;

	while ((pos = comanda.find(delimiter)) != std::string::npos) {
		token = comanda.substr(0, pos);
		if (token != "")
		{
			cuvinte.push_back(token);
		}
		comanda.erase(0, pos + delimiter.length());
	}

	if (pos == std::string::npos && comanda != "")
		cuvinte.push_back(comanda);

	if (cuvinte.size() != 3)
	{
		cout << "\ncomanda incorecta!";
		return;
	}
	
	int rez = DBCommands::ImportTable(db, cuvinte[1], cuvinte[2]);
	if (rez == 1)
		cout << "\nImportarea tabelei s-a realizat cu succes";
	return;
}

void CommandExec(DataBase* db, string comanda)
{
	string firstWord;

	size_t space_pos = comanda.find(" ");
	if (space_pos == string::npos)
		space_pos = comanda.length();
	if (space_pos != std::string::npos)
	{
		firstWord = comanda.substr(0, space_pos);
	}

	if (db == nullptr)
	{
		cout << "\nBaza de date inexistenta!";
		return;
	}

	if (firstWord == "INSERT")
	{
		insertIntoTable(db, comanda);
		return;
	}
	if (firstWord == "SELECT")
	{
		selectFromTable(db, comanda);
		return;
	}
	if (firstWord == "DELETE")
	{
		deleteFromTable(db, comanda);
		return;
	}
	if (firstWord == "CREATE")
	{
		createTable(db, comanda);
		return;
	}
	if (firstWord == "DROP")
	{
		dropTable(db, comanda);
		return;
	}
	if (firstWord == "DISPLAY")
	{
		displayTable(db, comanda);
		return;
	}
	if (firstWord == "IMPORT")
	{
		importTable(db, comanda);
		return;
	}
	cout << "\ncomanda incorecta!";
}

void main(int argc, char* argv[])
{
	string comanda;
	InitDB* initdb = new InitDB();
	DataBase* db = initdb->GetDatabase();


	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			std::ifstream f(argv[i], ios::in);
			
			if (f)
			{
				
				while (std::getline(f,comanda))
				{
					for (auto& c : comanda) c = toupper(c);
					CommandExec(db, comanda);
				}
				f.close();
			}
			else
			{
				cout << "\nFisierul "<< argv[i]<<" nu exista";
				return;
			}
		}
	}
	else
	{
		do
		{
			cout << "\nIntroduceti comanda dorita,\"EXIT\" pentru iesire:";
			std::getline(std::cin, comanda);
			for (auto& c : comanda) c = toupper(c);
			comanda = trim_left_copy(comanda);
			if (comanda != "EXIT")
			{
				CommandExec(db, comanda);
			}
		} while (comanda != "EXIT");
	}
}
