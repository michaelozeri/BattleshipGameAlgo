#include "SmartBattleshipGameAlgo.h"
#include "../Common/GameBoardUtils.h"

pair<int, int> SmartBattleshipGameAlgo::attack()
{
	pair<int, int> currentAttack;
	srand(time(NULL));

	if(m_randomMode)
	{
		int randomLocation = rand() % m_attacksRemain.size();
		currentAttack = m_attacksRemain[randomLocation];
		m_attacksRemain.erase(m_attacksRemain.begin() + randomLocation-1); // liga!
		return currentAttack;
	}
	//sink ship mode
	

	return {};
}

/*
this function is called at startup to update each players board game
*/
void SmartBattleshipGameAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	m_attacksRemain.size();
	m_myPlayerNum = player;
	m_board = GameBoardUtils::InitializeNewEmptyBoard();
	m_cannotAttackBoard = GameBoardUtils::InitializeNewEmptyBoard();
	for (int i = 0; i<ROWS; i++)
	{
		for (int j = 0; j<COLS; j++)
		{
			m_cannotAttackBoard[i][j] = 'X'; //X means we can attack here. 'V' means not
		}
	}
	GameBoardUtils::CloneBoardToPlayer(board, m_myPlayerNum, m_board);
	//prepering a matrix to know where not to attack
	GameBoardUtils::MarkCannotAttack(m_cannotAttackBoard, m_myPlayerNum, m_board);
	for (int i = 0; i<ROWS; i++)
	{
		for (int j = 0; j<COLS; j++)
		{
			if(m_cannotAttackBoard[i][j] == 'X') //if we can attack at that spot
			{
				m_attacksRemain.push_back(pair<int, int>(i,j));
			}
		}
	}
}

void SmartBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if(m_myPlayerNum == player)
	{
		if (m_randomMode)
		{
			if(result == AttackResult::Hit)
			{
				m_randomMode = 0;
			}
			//else - sink || Miss - stay random
			else
			{
				m_randomMode = 1;
			}
		}
		else
		{
			if(result == AttackResult::Hit)
			{
				
			}
		}
	}
	else
	{
		
	}
}

SmartBattleshipGameAlgo::~SmartBattleshipGameAlgo()
{
	
}

bool SmartBattleshipGameAlgo::init(const string& path)
{
	return true; //TOOD: is ok here to return true always? because doesnt need to init file
}

/*
 * for exporting this algo in main
 */
IBattleshipGameAlgo* GetAlgorithm()
{
	return (new SmartBattleshipGameAlgo());
}
