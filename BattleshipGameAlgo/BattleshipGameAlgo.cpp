// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BattleshipGameAlgo.h"
#include "main.h"
#include <algorithm>

//non - default constructor
BattleshipGameAlgo::BattleshipGameAlgo(const std::string & attackPath, const int playerNum) : _currentscore(0), _myPlayerNum(playerNum), _attacksDone(false), _attck_receiver(attackPath), _board(nullptr)
{
	cout << "non - default constructor" << endl;
}

/*
 * \brief 
 * \return (0,0) in case of EOF. (-1, -1) in case of any failure
 */
std::pair<int,int> BattleshipGameAlgo::attack()
{
	if(_attacksDone)
	{
		return{ 0,0 };
	}

	pair<int, int> attack = _attck_receiver.GetNextLegalAttack();
	if(attack.first == 0 && attack.second ==0)
	{
		_attacksDone = true;
		_attck_receiver.Dispose();
	}
	return attack;
}

/*
this function is called at startup to update each players board game
*/
void BattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols) 
{
	_board = utils.AllocateNewBoard();
	utils.CloneBoardToPlayer(board, _myPlayerNum, _board);
	std::cout << "finished copying array" << std::endl;
}

/*
this function only updates board of player & adds score if needed.
checking of if the game is ended will be at main function
*/
void BattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) 
{
	if (_myPlayerNum == player )
	{
		char check = _board[row][col];
		if(result != AttackResult::Miss)
		{
			_board[row][col] = '@';
		}
		if (result == AttackResult::Sink) 
		{
			switch (tolower(check)) 
			{
			case RubberBoatB:
				_currentscore += RubberBoatPoints;
				break;
			case RocketShipB:
				_currentscore += RocketShipPoints;
				break;
			case SubmarineB:
				_currentscore += SubmarinePoints;
				break;
			case DestroyerB:
				_currentscore += DestroyerPoints;
				break;
			default: ;
			}
		}
	}
}

//getter for if the attacks are finished
bool BattleshipGameAlgo::AttacksDone() const
{
	return _attacksDone;
}

int BattleshipGameAlgo::GetSctore() const
{
	return _currentscore;
}

AttackReciever::AttackReciever(const string& attackPath) : path(attackPath)
{
	_file.open(attackPath);
}

int AttackReciever::ReadNextAttack(std::pair<int, int>& pair)
{
	string line;

	// Read next line
	std::getline(_file, line);

	// Error occured on reading and EOF didn't reach
	if (!_file && !_file.eof())
	{
		std::cout << "Error: Read from file " << path << " failure!" << std::endl;
		return -1;
	}

	// Remove all spaces in line
	line.erase(remove(line.begin(), line.end(), ' '), line.end());
	
	// Find place of delimiter ','
	string::size_type index = line.find(',');
	if (index == string::npos || index == 0 || index == line.length() - 1)
	{
		return 1; // Attack is invalid, no delimiter or invalid delimiter place
	}

	// Get string from 0 to index (exclude)
	string rowString = line.substr(0, index);
	
	//Get string from index +
	string colString = line.substr(index + 1);

	//convert attack string into int's
	int row, col;
	if (ConvertStringToIntSafely(rowString, row) != 0 || ConvertStringToIntSafely(colString, col) != 0)
	{
		//Convert failed
		return 1;
	}

	//checking if index is ok
	if ((row > 10) || (row < 1) || (col < 1) || (col > 10))
	{
		// Values are out of range
		return 1;
	}

	pair.first = row;
	pair.second = col;
	return 0; 
}

pair<int, int> AttackReciever::GetNextLegalAttack()
{
	std::string attackRow, attackCol;
	
	while(!_file.eof())
	{
		pair<int, int> attack;
		int result = ReadNextAttack(attack);
		if(result == 0) // Success read attack
		{
			return attack;
		}
		if(result == 1) // Attack is Invalid
		{continue; }

		if(result == -1) // Failure during reading 
		{
			return{ -1, -1 };
		}
	}
	return{ 0,0 };
}

int AttackReciever::ConvertStringToIntSafely(string& line, int& number) const
{
	try
	{
		number = stoi(line);
		return 0;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

void AttackReciever::Dispose()
{
	_file.close();
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

bool GameBordUtils::IsPlayerIdChar(int playerID, char current){
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

bool GameBordUtils::IsLegalBoradChar(char current){
	return IsPlayerIdChar(PlayerAID, current) || IsPlayerIdChar(PlayerBID, current);
}

void GameBordUtils::LoadLineToBoard(char** board, int row, int cols, const string& cs){
	char* currentRow = board[row];
	unsigned long long len = cs.length() < cols ? cs.length() : cols;
	for (size_t i = 0; i < len; i++)
	{
		char currentChar = cs[i];
		currentRow[i] = IsLegalBoradChar(currentChar) ? currentChar : BLANK;
	}
}

char** GameBordUtils::AllocateNewBoard(){
	char** board = new char*[ROWS];
	for (int i = 0; i < ROWS; ++i)
		board[i] = new char[COLS];
	return board;
}

void GameBordUtils::DeleteBoard(char** board){
	// Delete board array
	for (int i = 0; i < ROWS; ++i) {
		delete[] board[i];
	}
	delete[] board;
}

BoardFileErrorCode GameBordUtils::LoadBoardFromFile(char** board, int rows, int cols, const string& filePath){
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

	// TODO:  Validate char** board return BoardFileErrorCode::other error code
	return BoardFileErrorCode::Success;
}

void GameBordUtils::PrintBoard(ostream& stream, char** board, int rows, int cols){
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

void GameBordUtils::CloneBoardToPlayer(const char** full_board, int playerID, char** player_board){
	InitBoard(player_board, ROWS, COLS);

	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			player_board[i][j] = IsPlayerIdChar(playerID, full_board[i][j]) ? full_board[i][j] : player_board[i][j];
		}
	}
}

ShipDetatilsBoard::ShipDetatilsBoard(char** board, int playerID) : playerID(playerID), borad(board){
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			char cell = board[i][j];
			if(_utils.IsPlayerIdChar(playerID,cell))
			{
				switch (tolower(cell))
				{
				case RubberBoatB:
					RubberBoatCells++;
					break;
				case RocketShipB:
					RocketShipCells++;
					break;
				case SubmarineB:
					SubmarineCells++;
					break;
				case DestroyerB:
					DestroyeCells++;
					break;
				default:
					break;
				}
			}
		}
	}
}

/**
* \brief Attack on the current player
* \param attack 
* \return AtackResult enum instance and update the board
*/
AttackResult ShipDetatilsBoard::GetAttackResult(pair<int, int> attack)
{
	char cell = borad[attack.first][attack.second];
	if(_utils.IsPlayerIdChar(playerID, cell))
	{
		borad[attack.first][attack.second] = '@';
		switch (tolower(cell))
		{
		case 'b':
			RubberBoatCells--;
			return RubberBoatCells % RubberBoatW == 0 ? AttackResult::Sink : AttackResult::Hit;
		case 'p':
			RocketShipCells--;
			return RocketShipCells % RocketShipW == 0 ? AttackResult::Sink : AttackResult::Hit;
		case 'm':
			SubmarineCells--;
			return SubmarineCells % SubmarineW == 0 ? AttackResult::Sink : AttackResult::Hit;
		case 'd':
			DestroyeCells--;
			return DestroyeCells % DestroyerW == 0 ? AttackResult::Sink : AttackResult::Hit;
		default:
			// Restore the previous value - should not get here
			borad[attack.first][attack.second] = cell;
			break;
		}
	}
	return AttackResult::Miss;
}

bool ShipDetatilsBoard::IsLoose() const
{
	int sum = RubberBoatCells + RocketShipCells + SubmarineCells + DestroyeCells;
	return (sum == 0);
}
