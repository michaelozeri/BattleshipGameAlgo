// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "BattleshipGameAlgo.h"

//a logger instance to log all data. initialized at main
Logger AppLogger;


//non - default constructor
BattleshipGameAlgo::BattleshipGameAlgo(const std::string & attackPath, const int playerNum) : m_myPlayerNum(playerNum), m_attacksDone(false), m_board(nullptr)
{
	m_attackReceiver = new AttackReciever(attackPath);
}

/*
 * \brief 
 * \return (0,0) in case of EOF. (-1, -1) in case of any failure
 */
std::pair<int,int> BattleshipGameAlgo::attack()
{
	if(m_attacksDone)
	{
		return{ 0,0 };
	}

	pair<int, int> attack = m_attackReceiver->GetNextLegalAttack();
	if(attack.first == 0 && attack.second ==0)
	{
		m_attacksDone = true;
		m_attackReceiver->Dispose();
	}
	return attack;
}

/*
this function is called at startup to update each players board game
*/
void BattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols) 
{
	m_board = GameBordUtils::AllocateNewBoard();
	GameBordUtils::CloneBoardToPlayer(board, m_myPlayerNum, m_board);
}

/*
this function only updates board of player & adds score if needed.
checking of if the game is ended will be at main function
*/
void BattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) 
{
}

BattleshipGameAlgo::~BattleshipGameAlgo()
{
	GameBordUtils::DeleteBoard(m_board);
	delete m_attackReceiver;
}

//getter for if the attacks are finished
bool BattleshipGameAlgo::AttacksDone() const
{
	return m_attacksDone;
}


ShipDetatilsBoard::ShipDetatilsBoard(char** board, int playerID) : playerID(playerID), mainboard(board), RubberBoatCells(0), RocketShipCells(0), SubmarineCells(0), DestroyeCells(0), negativeScore(0)
{
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			char cell = board[i][j];
			if (_utils.IsPlayerIdChar(playerID, cell))
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
	AttackResult result = AttackResult::Miss;
	char cell = mainboard[attack.first][attack.second];
	if(_utils.IsPlayerIdChar(playerID, cell))
	{
		mainboard[attack.first][attack.second] = HIT_CHAR;
		_utils.PrintBoard(AppLogger.logFile, mainboard, ROWS, COLS);
		switch (tolower(cell))
		{
		case 'b':
			RubberBoatCells--;
			result =  RubberBoatCells % RubberBoatW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? RubberBoatPoints : 0;
			break;
		case 'p':
			RocketShipCells--;
			result =  RocketShipCells % RocketShipW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? RocketShipPoints : 0;
			break;
		case 'm':
			SubmarineCells--;
			result = SubmarineCells % SubmarineW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? SubmarinePoints : 0;
			break;
		case 'd':
			DestroyeCells--;
			result = DestroyeCells % DestroyerW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? DestroyerPoints : 0;
			break;
		default:
			// Restore the previous value - should not get here
			mainboard[attack.first][attack.second] = cell;
			break;
		}
	}
	return result;
}

bool ShipDetatilsBoard::IsLoose() const
{
	int sum = RubberBoatCells + RocketShipCells + SubmarineCells + DestroyeCells;
	return (sum == 0);
}
