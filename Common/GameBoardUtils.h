#pragma once

#include "Contants.h"
#include <thread>

using namespace std;

/*
* a utility class for performing static functions at main function
*/
class GameBoardUtils {
	static BoardFileErrorCode ValidateGameBoard(char** board, int rows, int cols);
public:
	static void InitBoard(char** board, int rows, int cols);
	static bool IsPlayerIdChar(int playerID, char current);
	static int GetCharPlayerId(char current);
	static bool IsLegalBoradChar(char current);
	static void LoadLineToBoard(char** board, int row, int cols, const string& cs);
	static char** InitializeNewEmptyBoard();
	static void DeleteBoard(char** board);
	static BoardFileErrorCode LoadBoardFromFile(char** board, int rows, int cols, const string& filePath);
	static void PrintBoard(ostream& stream, char** board, int rows, int cols);
	static void CloneBoardToPlayer(const char** full_board, int playerID, char** player_board);
	static void CloneBoard(char** full_board, char** player_board);
	static void MarkCannotAttack(char** markBoard, int playernum, char** mainBoard);
	static string GetFilePathBySuffix(int argc, string customPath, string filesuffix, bool direxists = true);
	static bool DirExists(const std::string& dirName_in);
	static char** ClonePlayerBoard(const char** fullBoard, int i);
	static void ChangeFontSize();
};