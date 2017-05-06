#pragma once

#include "IBattleshipGameAlgo.h"


/*
* a class that represents a player at the game
* holds the current information of the player while playing the game
*/
class NaiveBattleshipGameAlgo : public IBattleshipGameAlgo 
{
public:
	
	//IBattleshipGameAlgo	
	void setBoard(int player,const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	~NaiveBattleshipGameAlgo();
	virtual bool init(const std::string& path)
	{
		return false;
	}
private:
	
	int m_myPlayerNum; //My player number
	//boolean value setting if i finished my attacks
	bool m_attacksDone;
	//my board game
	char** m_board;
	//a board for marking where cant we attack
	char** m_cannotAttackBoard;
	//holder of current attack spot
	std::pair<int, int> m_currentAttack;
};