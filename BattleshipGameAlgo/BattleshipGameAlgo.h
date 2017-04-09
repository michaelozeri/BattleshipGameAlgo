#pragma once

#include "IBattleshipGameAlgo.h"
#include "IOLib.h"
#include "Contants.h"
#include "GameBordUtils.h"

//a logger instance to log all data. initialized at main
extern Logger AppLogger;


/*
* a utility class for handeling all attack functionalities
*/
class AttackReciever{
	string path;
	ifstream _file;
public:
	AttackReciever(const string& attackPath);
	int ReadNextAttack(std::pair<int, int>& pair);
	pair<int, int> GetNextLegalAttack();
	int ConvertStringToIntSafely(string& line, int& number) const;
	void Dispose();
};



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

	ShipDetatilsBoard(char** board, int playerID);
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
	int GetSctore() const;

	//IBattleshipGameAlgo	
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result

private:
	int m_currentScore; //my score until now
	int m_myPlayerNum;
	bool m_attacksDone;
	GameBordUtils m_utils; //TODO: static class call instead of member
	Logger m_appLogger; //TODO: remove
	char** m_board; //my board game
	AttackReciever m_attackReceiver;
};



