#include "GameBoardUtils.h"
#include "IOLib.h"
#include "Contants.h"
#include <thread>
#include <Windows.h>

void GetWrongSizeErrMessage(char type, int player)
{
	char playerChar = player == PlayerAID ? 'A' : 'B';
	printf("Wrong size or shape for ship %c for player %c\n", type, playerChar);
}

int GetBoatTypeSizeFromChar (char type)
{
	switch (type)
	{
	case 'B':
	case 'b':
		return RubberBoatW;
	case 'P':
	case 'p':
		return RocketShipW;
	case 'M':
	case 'm':
		return SubmarineW;
	case 'D':
	case 'd':
		return DestroyerW;
	default: return 0;
	}
}

// Recursion function. Return if adjacent exist and mark all current boat with X
bool MarkAllBoat(char** board, int i, int j, char type)
{
	if (i < 0 || i >= ROWS || j < 0 || j >= COLS) // Stop recursion condition
	{
		return false;
	}

	char currentCell = board[i][j];
	if (currentCell == BLANK)
		return false;
	if (currentCell == 'X')
		return false;
	if (board[i][j] != type)
	{
		return true; // Adjacent exist - detect another boat of different type
	}

	board[i][j] = 'X'; // Mark current boat
	bool left = MarkAllBoat(board, i, j - 1, type);
	bool right = MarkAllBoat(board, i, j + 1, type);
	bool up = MarkAllBoat(board, i - 1, j, type);
	bool down = MarkAllBoat(board, i + 1, j, type);

	return left || right || up || down;
}

/*
* this function checks the ship size to the right or down as chosen
*direction 1 = check to the right
*direction 0 = check down
*/
bool CheckShipSize(char** board, char direction, int i, int j, char type)
{
	int boardTypeSize = GetBoatTypeSizeFromChar(type);
	int sizecheck = boardTypeSize - 1;

	int size = 0; // Will hold the current size of the boat
	
	if (direction) //if checking to the right
	{
		size_t a = j;
		for (size_t k = j; k <= std::fmin(j + sizecheck, COLS - 1); k++)
		{
			if (board[i][k] != type) // Check if next cell in right is ok - should be ok for all loop
			{
				return false;
			}
			if(i < ROWS-1 && board[i+1][k] == type) // check if there is no cell down of same type - should be flase
			{
				return false;
			}
			size++; // Increment boat size in case all is ok
			
			a = k + 1;
		}
		if(a < COLS-1 && board[i][a] == type) //check if boat is not larger than it should be from right
		{
			return false;
		}
		return size == boardTypeSize; // Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
	}
	else //direction of check is down
	{
		size_t a = i;
		for (size_t k = i; k <= fmin(i + sizecheck, ROWS - 1); k++)
		{
			if (board[k][j] != type)// Check if next cell down is ok - should be ok for all loop
			{
				return false;
			}
			if (j < COLS - 1 && board[k][j + 1] == type) // check if there is no cell right of same type - should be flase
			{
				return false;
			}
			if (j > 0 && board[k][j - 1] == type) // check if there is no cell left of same type - should be flase
			{
				return false;
			}
			size++; // Increment boat size in case all is ok
			a = k + 1;
		}
		if (a < ROWS - 1 && board[a][j] == type) //check if boat is not larger than it should be
		{
			return false;
		}
		return size == boardTypeSize; // Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
	}
}

/*
* this function validates the game board and prints by order the errors in the game
*/
BoardFileErrorCode GameBoardUtils::ValidateGameBoard(char** board, int rows, int cols)
{ 
	int playerAboatNum = 0; // Holds valid boat num from player A
	int playerBboatNum = 0; // Holds valid boat num from player B
	
	//bitwise or these values to not go from true -> false
	bool shapeB = false;
	bool shapeP = false;
	bool shapeM = false;
	bool shapeD = false;
	bool shapeb = false;
	bool shapep = false;
	bool shapem = false;
	bool shaped = false;
	bool adjacentErr = false;

	//checking size of ships
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++)
		{
			char cell = board[i][j];
			if (cell != 'X' && cell != BLANK)  //if i didnt check that place already and not BLANK
			{
				bool checkright, checkdown;
				checkright = CheckShipSize(board, 1, i, j, cell); //check size to the right
				checkdown = CheckShipSize(board, 0, i, j, cell); //check size down

				bool isBoatOfValidSize = GetBoatTypeSizeFromChar(cell) == 1 ? checkright & checkdown : checkright^checkdown; //xor because we want only one of them to be ok
				bool adjacent = MarkAllBoat(board, i, j, cell); // Flag which specified if there is another boat of different type or different player that adjacent to the current boat
				adjacentErr = adjacentErr || adjacent;

				if(isBoatOfValidSize) // Update boat number
				{
					if(IsPlayerIdChar(PlayerAID, cell))
					{
						playerAboatNum++; // Add valid ship for player A
					}
					else if(IsPlayerIdChar(PlayerBID, cell))
					{
						playerBboatNum++; // Add valid ship for player B
					}
				}

				else // Update Error ofType Wrong size or shape for ship <char> for player <ID>
				{
					switch (cell)
					{
					case 'B':
						shapeB = true; break;
					case 'b':
						shapeb = true; break;
					case 'P':
						shapeP = true; break;
					case 'p':
						shapep = true; break;
					case 'M':
						shapeM = true; break;
					case 'm':
						shapem = true; break;
					case 'D':
						shapeD = true; break;
					case 'd':
						shaped = true; break;
					default: ;
					}
				}
			}
		}
	}
	
	// Error Printing
	if(shapeB)
	{
		GetWrongSizeErrMessage('B', PlayerAID);
	}
	if(shapeP)
	{
		GetWrongSizeErrMessage('P', PlayerAID);
	}

	if( shapeM )
	{
		GetWrongSizeErrMessage('M', PlayerAID);
	}
	if(shapeD)
	{
		GetWrongSizeErrMessage('D', PlayerAID);
	}
	if( shapeb)
	{
		GetWrongSizeErrMessage('b', PlayerBID);
	}
	if(shapep)
	{
		GetWrongSizeErrMessage('p', PlayerBID);
	}
	if(shapem)
	{
		GetWrongSizeErrMessage('m', PlayerBID);
	}
	if(shaped)
	{
		GetWrongSizeErrMessage('d', PlayerBID);
	}

	// Print num of legal ships Errors
	if(playerAboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooFewShipsA << endl;
	}
	if (playerAboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooManyShipsA << endl;
	}
	if (playerBboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooFewShipsB << endl;
	}
	if (playerBboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooManyShipsB << endl;
	}
	
	// Print adjacent Error
	if (adjacentErr) 
	{
		cout << AdjacentERR << endl;
	}

	bool isNotLegalBoard = shapeB || shapeP || shapeM || shapeD || shapeb || shapep || shapem || shaped
		|| adjacentErr || playerAboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER || playerBboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER;
	return isNotLegalBoard ? BoardFileErrorCode::UnknownError : BoardFileErrorCode::Success;
}

void GameBoardUtils::InitBoard(char** board, int rows, int cols)
{
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			board[i][j] = BLANK;
		}
	}
}

bool GameBoardUtils::IsPlayerIdChar(int playerID, char current) {
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

int GameBoardUtils::GetCharPlayerId(char current)
{
	if (IsPlayerIdChar(PlayerAID, current))
	{
		return PlayerAID;
	}
	else if (IsPlayerIdChar(PlayerBID, current) )
		return PlayerBID;

	return 2;
}

bool GameBoardUtils::IsLegalBoradChar(char current) {
	return IsPlayerIdChar(PlayerAID, current) || IsPlayerIdChar(PlayerBID, current);
}

void GameBoardUtils::LoadLineToBoard(char** board, int row, int cols, const string& cs) {
	char* currentRow = board[row];

	int lenght_int = static_cast<int>(cs.length());
	int len = lenght_int < cols ? lenght_int : cols;
	for (int i = 0; i < len; i++)
	{
		char currentChar = cs[i];
		currentRow[i] = IsLegalBoradChar(currentChar) ? currentChar : BLANK;
	}
}

char** GameBoardUtils::InitializeNewEmptyBoard() {
	char** board = new char*[ROWS];
	for (int i = 0; i < ROWS; ++i)
		board[i] = new char[COLS];
	return board;
}

void GameBoardUtils::DeleteBoard(char** board) {
	// Delete board array
	for (int i = 0; i < ROWS; ++i) {
		delete[] board[i];
	}
	delete[] board;
}


BoardFileErrorCode GameBoardUtils::LoadBoardFromFile(char** board, int rows, int cols, const string& filePath) 
{
	BoardFileErrorCode errcode;
	//set all board to blank
	InitBoard(board, rows, cols);

	FileReader fileReader(filePath);

	int row = 0;
	while (!fileReader.IsEof() && row < 10)
	{
		string line;
		fileReader.ReadLine(line);
		LoadLineToBoard(board, row, cols, line);
		row++;
	}

	fileReader.CloseFile();
	
	// Clone current board, becaue ValidateGameBoard changed the board
	char** cloneBoard = GameBoardUtils::InitializeNewEmptyBoard();
	GameBoardUtils::CloneBoard(board, cloneBoard);
	errcode = ValidateGameBoard(cloneBoard, ROWS, COLS);

	// Delete clone board
	DeleteBoard(cloneBoard);

	return errcode;
}



void GameBoardUtils::PrintBoard(ostream& stream, char** board, int rows, int cols) 
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			stream << board[i][j];
		}
		stream << endl;
	}
}

void GameBoardUtils::CloneBoardToPlayer(const char** full_board, int playerID, char** player_board) {
	InitBoard(player_board, ROWS, COLS);

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			player_board[i][j] = IsPlayerIdChar(playerID, full_board[i][j]) ? full_board[i][j] : player_board[i][j];
		}
	}
}

void GameBoardUtils::CloneBoard(char** full_board, char** player_board) {
	InitBoard(player_board, ROWS, COLS);

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			player_board[i][j] = full_board[i][j];
		}
	}
}

void GameBoardUtils::MarkCannotAttack(char** markBoard, int playernum,char** mainBoard)
{
	for (int i = 0; i<ROWS; i++)
	{
		for (int j = 0; j<COLS; j++)
		{
			if (IsPlayerIdChar(playernum, mainBoard[i][j]))
			{
				markBoard[i][j] = 'V';
				//mark left of me
				if((j>0)&&(!IsPlayerIdChar(playernum,mainBoard[i][j-1])))
				{
					markBoard[i][j-1] = 'V';
				}
				//mark right of me
				if ((j<COLS-1) && (!IsPlayerIdChar(playernum, mainBoard[i][j+1])))
				{
					markBoard[i][j+1] = 'V';
				}
				//mark above me
				if((i>0)&&(!IsPlayerIdChar(playernum,mainBoard[i-1][j])))
				{
					markBoard[i-1][j] = 'V';
				}
				//mark below me
				if ((i<ROWS-1) && (!IsPlayerIdChar(playernum, mainBoard[i + 1][j])))
				{
					markBoard[i + 1][j] = 'V';
				}
			}
		}
	}
}

/*
* @param argc - of main program
* @param argv - of main program
* @param filesuffix - the suffix of the file we are searching for
* @return path to .sboard file (non-default / "" in case the working directory is chosen )
* @return "ERR" in case of error / file not found
*/
string GameBoardUtils::GetFilePathBySuffix(int argc, string customPath, string filesuffix, bool direxists) //TODO: check this is working after system() remove
{
	char currentdirectory[_MAX_PATH];
	//reference: taken from : http://stackoverflow.com/questions/19691058/c-visual-studio-current-working-directory
	_fullpath(currentdirectory, ".\\", _MAX_PATH); // obtain current directory
	string filename, suffix;
	string delimiter = ".";
	string nondefaultpath = customPath;
	string systemcallcommand;
	size_t pos;
	string templine;
	char buffer[4096];
	FILE* fp;
	if (argc > 1) {
		if (direxists) {
			fp = _popen(("2>NUL dir /a-d /b " + nondefaultpath).c_str(), "r");
			while (fgets(buffer, 4095, fp))
			{
				templine = string(buffer);
				pos = templine.find(delimiter);
				suffix = templine.substr(pos, templine.length());
				filename = templine.substr(0, pos);
				if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
					_pclose(fp);
					if (nondefaultpath.at(nondefaultpath.length() - 1) == '\\') {
						return nondefaultpath + filename + suffix; //argument is inserted with '\' at string ending
					}
					return nondefaultpath + '\\' + filename + suffix; //argument is inserted without '\' at ending
				}
			}
			
			//no more files to read from non - default path
			if (filesuffix == ".sboard") {
				cout << "Missing board file (*.sboard) looking in path: " << currentdirectory << endl;
			}
			else if (filesuffix == ".attack-a") {
				cout << "Missing attack file for player A(*.attack - a) looking in path: " << currentdirectory << endl;
			}
			else {
				cout << "Missing attack file for player B(*.attack - b) looking in path: " << currentdirectory << endl;
			}
			_pclose(fp);
		}
	}
	fp = _popen(("2>NUL dir /a-d /b " + (string)currentdirectory).c_str(), "r");
	while (fgets(buffer, 4095, fp))
	{
		templine = string(buffer);
		pos = templine.find(delimiter);
		suffix = templine.substr(pos, templine.length());
		filename = templine.substr(0, pos);
		if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
			_pclose(fp);
			if (nondefaultpath.at(nondefaultpath.length() - 1) == '\\') {
				return nondefaultpath + filename + suffix; //argument is inserted with '\' at string ending
			}
			return nondefaultpath + '\\' + filename + suffix; //argument is inserted without '\' at ending
		}
	}

	cout << filesuffix + " file was not found in working directory" << endl;
	_pclose(fp);
	return "ERR";
}

//taken from: http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
bool GameBoardUtils::DirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

char** GameBoardUtils::ClonePlayerBoard(const char** fullBoard, int i)
{
	char** playerBoard = GameBoardUtils::InitializeNewEmptyBoard();
	GameBoardUtils::CloneBoardToPlayer(fullBoard, i, playerBoard);
	return playerBoard;
}

void GameBoardUtils::ChangeFontSize()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX orig = { sizeof(orig) };

	if (GetCurrentConsoleFontEx(hStdout, FALSE, &orig))
	{
		//AppLogger.logFile << "Got\n"; //TODO: restore all instances here of applogger
	}
	else
	{
		//AppLogger.logFile << GetLastError
	}

	orig.dwFontSize.X = 12;
	orig.dwFontSize.Y = 16;

	if (SetCurrentConsoleFontEx(hStdout, FALSE, &orig))
	{
		//AppLogger.logFile << "Set\n";
	}
	else
	{
		//AppLogger.logFile << endl << GetLastError();
	}

}

