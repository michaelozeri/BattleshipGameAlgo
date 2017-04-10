// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BattleshipGameAlgo.h"

//a logger instance to log all data. initialized at main
Logger AppLogger;


//non - default constructor
BattleshipGameAlgo::BattleshipGameAlgo(const std::string & attackPath, const int playerNum) : m_currentScore(0), m_myPlayerNum(playerNum), m_attacksDone(false), m_attackReceiver(attackPath), m_board(nullptr){}

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

	pair<int, int> attack = m_attackReceiver.GetNextLegalAttack();
	if(attack.first == 0 && attack.second ==0)
	{
		m_attacksDone = true;
		m_attackReceiver.Dispose();
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
	if (m_myPlayerNum == player )
	{
		GameBordUtils::PrintBoard(AppLogger.logFile, m_board,ROWS, COLS);

		char check = m_board[row][col]; //TODO: mordi change check to main board instead
		if(result != AttackResult::Miss)
		{
			m_board[row][col] = '@';
		}
		if (result == AttackResult::Sink) 
		{
			switch (tolower(check)) 
			{
			case RubberBoatB:
				m_currentScore += RubberBoatPoints;
				break;
			case RocketShipB:
				m_currentScore += RocketShipPoints;
				break;
			case SubmarineB:
				m_currentScore += SubmarinePoints;
				break;
			case DestroyerB:
				m_currentScore += DestroyerPoints;
				break;
			default: ;
			}
		}
	}
}

//getter for if the attacks are finished
bool BattleshipGameAlgo::AttacksDone() const
{
	return m_attacksDone;
}

int BattleshipGameAlgo::GetSctore() const
{
	return m_currentScore;
}


ShipDetatilsBoard::ShipDetatilsBoard(char** board, int playerID) : playerID(playerID), mainboard(board){
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
	char cell = mainboard[attack.first][attack.second];
	if(_utils.IsPlayerIdChar(playerID, cell))
	{
		mainboard[attack.first][attack.second] = '@';
		_utils.PrintBoard(AppLogger.logFile,mainboard,ROWS,COLS); 
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
			mainboard[attack.first][attack.second] = cell;
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
