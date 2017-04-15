#pragma once

#include "IBattleshipGameAlgo.h"
#include "IOLib.h"
#include "GameBordUtils.h"
#include "AttackReciever.h"

//a logger instance to log all data. initialized at main
extern Logger AppLogger;

/*
* a utility class for keeping track of game state
*/
class ShipDetatilsBoard 
{
	GameBordUtils _utils;
public:
	int playerID;
	char** mainboard;

	int RubberBoatCells;
	int RocketShipCells;
	int SubmarineCells;
	int DestroyeCells;

	int negativeScore; // Save the score of the other player
	ShipDetatilsBoard(char** board, int playerID);
	void SetSinkCharRec(int first, int second) const;
	AttackResult GetAttackResult(pair<int, int> attack);
	bool IsLoose() const;
};

/*
* a class that represents a player at the game
* holds the current information of the player while playing the game
*/
class BattleshipGameAlgo : public IBattleshipGameAlgo 
{
public:
	//constructor
	BattleshipGameAlgo(const std::string& attackPath, const int playerNum);

	//Getters
	bool AttacksDone() const;

	//IBattleshipGameAlgo	
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	~BattleshipGameAlgo();
private:
	//My player number
	int m_myPlayerNum;
	//boolean value setting if i finished my attacks
	bool m_attacksDone;
	//my board game
	char** m_board; 
	AttackReciever* m_attackReceiver;
};