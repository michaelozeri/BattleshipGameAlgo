#pragma once

#include "IBattleshipGameAlgo.h"
#include "IOLib.h"
#include "Contants.h"


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
* a utility class for performing static functions at main function
*/
class GameBordUtils{
public:
	static void InitBoard(char** board, int rows, int cols);
	static bool IsPlayerIdChar(int playerID, char current);
	static bool IsLegalBoradChar(char current);
	static void LoadLineToBoard(char** board, int row, int cols, const string& cs);
	static char** AllocateNewBoard();
	static void DeleteBoard(char** board);
	static BoardFileErrorCode LoadBoardFromFile(char** board, int rows, int cols, const string& filePath);
	static void PrintBoard(ostream& stream, char** board, int rows, int cols);
	static void CloneBoardToPlayer(const char** full_board, int playerID, char** player_board);
};

/*
* a utility class for keeping track of game state
*/
class ShipDetatilsBoard{
	GameBordUtils _utils;
public:
	int playerID;
	char** borad;

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
class BattleshipGameAlgo : public IBattleshipGameAlgo {
private:
	int _currentscore; //my score until now
	int _myPlayerNum;
	bool _attacksDone;
	GameBordUtils utils;
	Logger app_logger_;
	AttackReciever _attck_receiver;
public:
	char** _board; //my board game
	BattleshipGameAlgo(const std::string& attackPath, const int playerNum);
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	bool AttacksDone() const;
	int GetSctore() const;
};



