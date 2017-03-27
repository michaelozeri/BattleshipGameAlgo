#pragma once

#include "IBattleshipGameAlgo.h"

//class for each player
class BattleshipGameAlgo : public IBattleshipGameAlgo {
public:
	char** board; //other player board
	int score; // my score
	virtual void setBoard(const char** board, int numRows, int numCols); // called once to notify player on his board
	virtual std::pair<int, int> attack(); // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result); // notify on last move result
};