#pragma once
#include "IBattleshipGameAlgo.h"
#include <vector>

using namespace std;
/*
* a class that represents a player at the game
* holds the current information of the player while playing the game
*/
class SmartBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	//IBattleshipGameAlgo	
	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	~SmartBattleshipGameAlgo();
private:
	int m_randomMode = 1;
	//My player number
	int m_myPlayerNum;
	//my board game
	char** m_board;
	//a board for marking where cant we attack
	char** m_cannotAttackBoard;
	//function for generating a random attack
	vector<pair<int, int>> m_attacksRemain;
};