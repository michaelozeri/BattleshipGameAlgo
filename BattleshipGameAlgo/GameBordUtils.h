#pragma once

#include <string>
#include <ostream>
#include "Contants.h"

using namespace std;

/*
* a utility class for performing static functions at main function
*/
class GameBordUtils {
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