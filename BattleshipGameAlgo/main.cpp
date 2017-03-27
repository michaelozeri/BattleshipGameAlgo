#include "stdafx.h"

#include "IBattleshipGameAlgo.h"

#define ERR1A "Too many ships for player A"
#define ERR2A "Too few ships for player A"
#define ERR1B "Too many ships for player B"
#define ERR2B "Too few ships for player B"
#define ERR "Adjacent Ships on Board"

int main(int argc, char* argv[]) {
	std::string path; //path to enter the non-default path to files
	if (argc > 1) {
		path = argv[1];
	}
	std::cout << path << std::endl;
	char c;
	std::cin >> c;
	return 0;
}