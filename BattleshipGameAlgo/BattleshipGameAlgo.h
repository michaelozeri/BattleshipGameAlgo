#pragma once

#include "IBattleshipGameAlgo.h"
#include "IOLib.h"
#include "Contants.h"



class AttackReciever
{
	string path;
	ifstream _file;
public:
	AttackReciever(const string& attackPath);
	int ReadNextAttack(std::pair<int, int>& pair);
	pair<int, int> GetNextLegalAttack();
	int ConvertStringToIntSafely(string& line, int& number) const;
	void Dispose();
};

class GameBordUtils
{
public:
	void InitBoard(char** board, int rows, int cols);
	bool IsPlayerIdChar(int playerID, char current) const;
	bool IsLegalBoradChar(char current) const;
	void LoadLineToBoard(char** board, int row, int cols, const string& cs);


	char** GetNewBoard();
	void DeleteBoard(char** board);
	BoardFileErrorCode LoadBoardFromFile(char** board, int rows, int cols, const string& filePath);
	void PrintBoard(ostream& stream, char** board, int rows, int cols) const;
	void CloneBoardToPlayer(const char** full_board, int playerID, char** player_board);
};


//class for each player
class BattleshipGameAlgo : public IBattleshipGameAlgo {
private:
	int _currentscore; //my score until now
	int _myPlayerNum;
	bool _attacksDone;
	GameBordUtils utils;
	Logger app_logger_;
	AttackReciever _attck_receiver;
public:
	char** _board; //my board game - including opponent (i get notified on attacks on me)
	BattleshipGameAlgo(const std::string& attackPath, const int playerNum);
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	bool FinishedAttacks() const;
	int GetSctore() const;
};

class ShipDetatilsBoard
{
	GameBordUtils _utils;
public:
	int playerID;
	char** borad;

	int RubberBoatCells;
	int RocketShipCells;
	int SubmarineCells;
	int DestroyeCells;

	ShipDetatilsBoard(char** board, int playerID);
	AttackResult GetAttackResult(pair<int,int> attack);
	bool IsLoose() const;
};
