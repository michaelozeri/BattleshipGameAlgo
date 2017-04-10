#include "stdafx.h"

#include "GameBordUtils.h"
#include "IOLib.h"

/*
* this function checks the ship size to the right or down as chosen
*direction 1 = check to the right
*direction 0 = check down
*/
bool CheckShipSize(char** board, char direction, int i, int j, char type) {
	int sizecheck;
	switch (type) {
	case 'B':
	case 'b':
		sizecheck = 0;
		break;
	case 'P':
	case 'p':
		sizecheck = 1;
		break;
	case 'M':
	case 'm':
		sizecheck = 2;
		break;
	case 'D':
	case 'd':
		sizecheck = 3;
		break;
	}
	if (direction) { //if checking to the right
		if (sizecheck + j > COLS - 1) {
			board[i][j] = 'X'; //not enough space for ship
			return false;
		}
		else {
			for (size_t k = j; k < j+sizecheck; k++){
				if (board[i][k] != type) {
					return false;
				}
				board[i][k] = 'X'; //mark as passed over
			}
		}
	}
	else {//direction of check is down
		if (sizecheck + i > ROWS - 1) {
			board[i][j] = 'X'; //not enough space for ship at all
			return false;
		}
		else {
			for (size_t k = i; k < i + sizecheck; k++) {
				if (board[k][j] != type) {
					return false;
				}
				board[k][j] = 'X'; //mark as passed over
			}
		}
	}
	return true; //all is ok with the ship size
}

/*
* this function validates the game board and prints by order the errors in the game
*/
BoardFileErrorCode ValidateGameBoard(char** board, int rows, int cols) { //TODO: mordi, finish
	int tempchecksize = 0;
	bool checkright, checkdown;
	//bitwise or these values to not go from true -> false
	bool err1a = false;
	bool err2a = false;
	bool err1b = false;
	bool err2b = false;
	bool shape1B = false;
	bool shape1P = false;
	bool shape1M = false;
	bool shape1D = false;
	bool shape1b = false;
	bool shape1p = false;
	bool shape1m = false;
	bool shape1d = false;
	bool erralahson = false;
	//checking size of ships
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (board[i][j] != 'X') { //if i didnt check that place already
				//TODO: inside checkshipsize check that the size is not bigger then it should be.
				checkright = CheckShipSize(board, 1, i, j, board[i][j]);//check size to the right
				checkdown = CheckShipSize(board, 0, i, j, board[i][j]);;
				if (!(checkright^checkdown)) { //xor because we want only one of them to be ok

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