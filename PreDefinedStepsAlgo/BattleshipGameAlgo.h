#pragma once

#include "IBattleshipGameAlgo.h"
#include "../Common/AttackReciever.h"

//a logger instance to log all data. initialized at main
//extern Logger AppLogger;


/*
* a class that represents a player at the game
* holds the current information of the player while playing the game
*/
class PreDefinedBattleshipGameAlgo : public IBattleshipGameAlgo 
{
public:
	//defult constructor
	PreDefinedBattleshipGameAlgo();
	//Getters
	bool AttacksDone() const;
	//IBattleshipGameAlgo	
	void setBoard(int player,const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	~PreDefinedBattleshipGameAlgo();
	bool init(const std::string& path);
private:
	//My player number
	int m_myPlayerNum;
	//boolean value setting if i finished my attacks
	bool m_attacksDone;
	//my board game
	char** m_board; 
	AttackReciever* m_attackReceiver;
};