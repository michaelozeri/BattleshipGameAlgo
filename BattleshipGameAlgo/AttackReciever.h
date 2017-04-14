#pragma once

#include <iostream>
#include <fstream>
using namespace std;

/*
* a utility class for handeling all attack functionalities
*/
class AttackReciever {
private:
	string path;
	ifstream _file;
public:
	AttackReciever(const string& attackPath);
	int ReadNextAttack(std::pair<int, int>& pair);
	pair<int, int> GetNextLegalAttack();
	int ConvertStringToIntSafely(string& line, int& number) const;
	void Dispose();
	~AttackReciever();
};