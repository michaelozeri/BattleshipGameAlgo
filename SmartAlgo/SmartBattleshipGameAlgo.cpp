#include "SmartBattleshipGameAlgo.h"
#include "../Common/GameBoardUtils.h"

std::pair<int, int> SmartBattleshipGameAlgo::attack()
{
	if(m_randomMode)
	{
	
	}
	else //sink ship mode
	{
		
	}

	return {};
}

/*
this function is called at startup to update each players board game
*/
void SmartBattleshipGameAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
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
				//TODO: insert into specific place where random out of it the attacks
			}
		}
	}
}