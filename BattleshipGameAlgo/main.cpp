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
#include "main.h"

using namespace std; //TODO: remove

int main(int argc, char* argv[]) {

	char fullGameBoard[ROWS][COLS];//this is the main game board - initialized to 10X10 matrix
	string path = nullptr; //path to enter the non-default path to files
	std::pair<int, int> temppair;
	AttackResult attackresult;
	bool skipA = false, skipB = false; //bool for knowing if to give another turn to A or B
	if (argc > 1) {
		path = argv[1];
	}

	//find *.sboard file at path.
	std::string temprow;

	std::ifstream mainboard(path);

	for (int i = 0; i < ROWS; i++) {
		getline(mainboard, temprow);
		temprow.copy(fullGameBoard[i], COLS-1, 0);
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
				attackresult = AttackResult::Hit;
				//update players
				playerA.notifyOnAttackResult(0, temppair.first, temppair.second, attackresult);
				playerB.notifyOnAttackResult(0, temppair.first, temppair.second, attackresult);
				if ((attackresult == AttackResult::Hit) || (attackresult == AttackResult::Sink)) {
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
				attackresult = AttackResult::Hit;
				//update players
				playerA.notifyOnAttackResult(1, temppair.first, temppair.second, attackresult);
				playerB.notifyOnAttackResult(1, temppair.first, temppair.second, attackresult);
				if ((attackresult == AttackResult::Hit) || (attackresult == AttackResult::Sink)) {
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
