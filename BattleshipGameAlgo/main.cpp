#include "stdafx.h"

#include "BattleshipGameAlgo.h"

#define ERR1A "Too many ships for player A"
#define ERR2A "Too few ships for player A"
#define ERR1B "Too many ships for player B"
#define ERR2B "Too few ships for player B"
#define ERR "Adjacent Ships on Board"

#include <fstream>

using namespace std; //TODO: remove

int main(int argc, char* argv[]) {

	char** fullGameBoard;

	BattleshipGameAlgo playerA();
	BattleshipGameAlgo playerB();

	string path; //path to enter the non-default path to files
	if (argc > 1) {
		path = argv[1];
	}
	else {

	}

	ifstream fin("michael.txt"); //default read from files

	getline(fin, path);

	cout << path << endl;

	//for debugging
	char c;
	:cin >> c;

	return 0;
}

private int LookForFiles() {

	return 0;
}