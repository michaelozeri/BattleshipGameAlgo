#include "AttackReciever.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

AttackReciever::AttackReciever(const string& attackPath) : path(attackPath)
{
	_file.open(attackPath);
}

int AttackReciever::ReadNextAttack(pair<int, int>& pair)
{
	string line;

	// Read next line
	getline(_file, line);
	line.erase(remove(line.begin(), line.end(), '\r'), line.end());

	// Error occured on reading and EOF didn't reach
	if (!_file && !_file.eof())
	{
		std::cout << "Error: Read from file " << path << " failure!" << std::endl;
		return -1;
	}

	//Remove all spaces in line
	line.erase(remove(line.begin(), line.end(), ' '), line.end());

	// Find place of delimiter ','
	string::size_type index = line.find(',');
	if (index == string::npos || index == 0 || index == line.length() - 1)
	{
		return 1; // Attack is invalid, no delimiter or invalid delimiter place
	}

	// Get string from 0 to index (exclude)
	string rowString = line.substr(0, index);

	//Get string from index +
	string colString = line.substr(index + 1);

	//convert attack string into int's
	int row, col;
	if (ConvertStringToIntSafely(rowString, row) != 0 || ConvertStringToIntSafely(colString, col) != 0)
	{
		//Convert failed
		return 1;
	}

	//checking if index is ok
	if ((row > 10) || (row < 1) || (col < 1) || (col > 10))
	{
		// Values are out of range
		return 1;
	}

	pair.first = row;
	pair.second = col;
	return 0;
}

pair<int, int> AttackReciever::GetNextLegalAttack()
{
	std::string attackRow, attackCol;

	while (!_file.eof())
	{
		pair<int, int> attack;
		int result = ReadNextAttack(attack);
		if (result == 0) // Success read attack
		{
			return attack;
		}
		if (result == 1) // Attack is Invalid
		{
			continue;
		}

		if (result == -1) // Failure during reading 
		{
			return{ -1, -1 };
		}
	}
	return{ 0,0 };
}

int AttackReciever::ConvertStringToIntSafely(string& line, int& number) const
{
	try
	{
		number = stoi(line);
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

void AttackReciever::Dispose()
{
	_file.close();
}

AttackReciever::~AttackReciever()
{
	if(_file.is_open())
	{
		Dispose();
	}
}
