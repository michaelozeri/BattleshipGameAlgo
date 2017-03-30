#include "stdafx.h"

#include "BattleshipGameAlgo.h"

#define ERR1A "Too many ships for player A"
#define ERR2A "Too few ships for player A"
#define ERR1B "Too many ships for player B"
#define ERR2B "Too few ships for player B"
#define ERR "Adjacent Ships on Board"
#define ROWS 10
#define COLS 10

#include <fstream>
#include "main.h" //TODO: remove?

int MakeMainBattleBoard(std::string path, std::string* fullGameBoard, int rows, int cols)
{
	std::cout << "Reading battleBoard from file: " << path << "board.sboard into class BattleBoard" << std::endl;
	std::ifstream fin(path + "board.sboard");
	//fin.ignore(); //skip newline and go the begining of matrix
	fullGameBoard = new std::string[rows];
	for (int i = 0; i <rows; ++i)
	{
		std::getline(fin, fullGameBoard[i]);
	}
	//TODO: check that fullgameboard is ok
	return 0;
}

using namespace std; //TODO: remove

int main(int argc, char* argv[]) {

	std::string fullGameBoard[ROWS];//this is the main game board 
	std::string path = nullptr; //path to enter the non-default path to files, set to nullptr at default
	std::pair<int, int> temppair;
	AttackResult tempattackresult;
	bool skipA = false, skipB = false; //bool for knowing if to give another turn to A or B
	if (argc > 1) {
		path = argv[1];
	}

	//find *.sboard file at path.
	//std::ifstream mainboard( path + "board.sboard"); //TODO: check if we can guess the filename or try open all file with extension
	//if (!mainboard) {
	//	std::cout << "didnt manage to open board on path given, trying on working directory" << std::endl;
	//	mainboard.open("*.sboard");
	//}

	//std::string temprow("hellllo");
	//for (int i = 0; i < ROWS; i++) {
	//	getline(mainboard, temprow);
	//	temprow.copy(fullGameBoard[i], COLS-1, 0);
	//}

	if (MakeMainBattleBoard(path, fullGameBoard, ROWS, COLS) == -1) {
		std::cout << "battle board recieved is bad, exiting program" << std::endl;
		return -1;
	}

	//fullGameBoard is filled up

	BattleshipGameAlgo playerA(path,0);
	BattleshipGameAlgo playerB(path,1);

	//main game play
	while (!playerA.FinishedAttacks() && !playerB.FinishedAttacks()) {
		if (!skipA) {
			//player A attackes
			temppair = playerA.attack();
			if ((temppair.first == 0) || (temppair.second == 0)) {
				std::cout << "got bad attack line - not atttacking (A)" << std::endl;
			}
			else {
				//calculate attack -TODO: mordi
				tempattackresult = AttackResult::Hit;
				//update players
				playerA.notifyOnAttackResult(0, temppair.first, temppair.second, tempattackresult);
				playerB.notifyOnAttackResult(0, temppair.first, temppair.second, tempattackresult);
				if ((tempattackresult == AttackResult::Hit) || (tempattackresult == AttackResult::Sink)) {
					skipB = true;
				}
			}
		}
		if (!skipB) {
			//player B attack();
			temppair = playerB.attack();
			if ((temppair.first == 0) || (temppair.second == 0)) {
				std::cout << "got bad attack line - not atttacking (A)" << std::endl;
			}
			else {
				//calculate attack -TODO: mordi
				tempattackresult = AttackResult::Hit;
				//update players
				playerA.notifyOnAttackResult(1, temppair.first, temppair.second, tempattackresult);
				playerB.notifyOnAttackResult(1, temppair.first, temppair.second, tempattackresult);
				if ((tempattackresult == AttackResult::Hit) || (tempattackresult == AttackResult::Sink)) {
					skipA = true;
				}
			}
		}
		skipA = false;
		skipB = false;
	}

	//for debugging- TODO: remove this
	char c;
	cin >> c;

	return 0;
}
