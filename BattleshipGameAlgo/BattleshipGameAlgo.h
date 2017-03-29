#pragma once

#include "IBattleshipGameAlgo.h"

//class for each player
class BattleshipGameAlgo : public IBattleshipGameAlgo {
private:
	char** _board; //my board game - including opponent (i get notified on attacks on me)
	int _currentscore; //my score until now
	std::ifstream _attack; //my attack file
	int _myPlayerNum;
	bool _attacksDone;
public:
	BattleshipGameAlgo();
	BattleshipGameAlgo(const std::string & attackPath, const int playerNum);
	virtual void setBoard(const char** board, int numRows, int numCols); // called once to notify player on his board
	virtual std::pair<int, int> attack(); // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result); // notify on last move result
	bool FinishedAttacks();
};