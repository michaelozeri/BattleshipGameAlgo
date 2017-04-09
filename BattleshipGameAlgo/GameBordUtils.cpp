#include "stdafx.h"

#include "GameBordUtils.h"
#include "IOLib.h"

/*
* this function checks that in the adjecant places around each ship there
* are no other ship
* @return - true if the board is not ok, otherwise false
*/
bool CheckAlahson(char** board,int i,int j,int rows,int cols) { //TODO: check for i,j in ooposite side of ending
	if (i = 0) {
		if (j = 0) {
			if ((board[i + 1][j + 1] != ' ')) {
				return true;
			}
		}
		else {
			if ((board[i + 1][j + 1] != ' ')||(board[i + 1][j - 1] != ' ')) {
				return true;
			}
		}
	}
	else { //i != 0
		if (j = 0) {
			if ((board[i - 1][j + 1] != ' ')|| (board[i + 1][j + 1] != ' ')) {
				return true;
			}
		}
		else {
			if ((board[i - 1][j + 1] != ' ') || (board[i + 1][j - 1] != ' ')|| (board[i - 1][j - 1] != ' ')|| (board[i + 1][j + 1] != ' ')) {
				return true;
			}
		}
	}
	return false;
}

/*
* this function validates the game board and prints by order the errors in the game
*/
BoardFileErrorCode ValidateGameBoard(char** board,int rows,int cols) { //TODO: michael, finish
	bool err1a = false;
	bool err2a = false;
	bool err3a = false;
	bool err1b = false;
	bool err2b = false;
	bool err3b = false;
	bool erralahson = false;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (!erralahson) {
				if (board[i][j] != ' ') {
					erralahson = CheckAlahson(board, i, j, rows, cols);
				}
			}
			
		}
	}
	if (erralahson) {
		cout << ERRALAHSON << endl;
	}
	return BoardFileErrorCode::Success;
}

void GameBordUtils::InitBoard(char** board, int rows, int cols)
{
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			board[i][j] = BLANK;
		}
	}
}

bool GameBordUtils::IsPlayerIdChar(int playerID, char current) {
	if (playerID == PlayerAID)
	{
		return current == RubberBoatA ||
			current == RocketShipA ||
			current == SubmarineA ||
			current == DestroyerA;
	}
	if (playerID == PlayerBID)
	{
		return  current == RubberBoatB ||
			current == RocketShipB ||
			current == SubmarineB ||
			current == DestroyerB;
	}
	return false;
}

bool GameBordUtils::IsLegalBoradChar(char current) {
	return IsPlayerIdChar(PlayerAID, current) || IsPlayerIdChar(PlayerBID, current);
}

void GameBordUtils::LoadLineToBoard(char** board, int row, int cols, const string& cs) {
	char* currentRow = board[row];
	unsigned long long len = cs.length() < cols ? cs.length() : cols;
	for (size_t i = 0; i < len; i++)
	{
		char currentChar = cs[i];
		currentRow[i] = IsLegalBoradChar(currentChar) ? currentChar : BLANK;
	}
}

char** GameBordUtils::AllocateNewBoard() {
	char** board = new char*[ROWS];
	for (int i = 0; i < ROWS; ++i)
		board[i] = new char[COLS];
	return board;
}

void GameBordUtils::DeleteBoard(char** board) {
	// Delete board array
	for (int i = 0; i < ROWS; ++i) {
		delete[] board[i];
	}
	delete[] board;
}


BoardFileErrorCode GameBordUtils::LoadBoardFromFile(char** board, int rows, int cols, const string& filePath) {

	BoardFileErrorCode errcode;
	//set all board to blank
	InitBoard(board, rows, cols);

	FileReader fileReader(filePath);

	int row = 0;
	while (!fileReader.IsEof() || row == 10)
	{
		string line;
		fileReader.ReadLine(line);
		LoadLineToBoard(board, row, cols, line);
		row++;
	}

	fileReader.CloseFile();

	errcode = ValidateGameBoard(board,ROWS,COLS);

	return errcode;
}



void GameBordUtils::PrintBoard(ostream& stream, char** board, int rows, int cols) {
	stream << "######Game Board######" << endl;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			stream << board[i][j];
		}
		stream << endl;
	}
	stream << "###End Game Board######" << endl;
}

void GameBordUtils::CloneBoardToPlayer(const char** full_board, int playerID, char** player_board) {
	InitBoard(player_board, ROWS, COLS);

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			player_board[i][j] = IsPlayerIdChar(playerID, full_board[i][j]) ? full_board[i][j] : player_board[i][j];
		}
	}
}