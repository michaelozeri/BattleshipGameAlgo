#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class FileReader
{
private:
	std::string filePath;
	std::ifstream fin;
public:
	FileReader(const std::string& path) :filePath(path)
	{
		fin.open(path);
	}
	string& ReadLine(string& line)
	{
		getline(fin, line);
		return line;
	}

	bool IsEof() const
	{
		return fin.eof();
	}

	void PrintFile()
	{
		string l;
		while (getline(fin, l))
		{
			cout << l << endl;
		}
	}

	void CloseFile()
	{
		fin.close();
	}
};

class Logger
{
public:
	std::ofstream logFile;

	void InitLogger(const std::string& logPath)
	{
		logFile.open(logPath);
	}
	void LoggerDispose()
	{
		logFile.close();
	}
};

