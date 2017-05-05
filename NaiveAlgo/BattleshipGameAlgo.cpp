// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "NaiveBattleshipGameAlgo.h"
#include <utility>
#include "../Common/GameBoardUtils.h"
#include "../Common/IOLib.h"

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

	//convert back to our matrix coordinates
	int i=m_currentAttack.first-1;
	int j = m_currentAttack.second-1;

	//find next attack
	while(i<ROWS)
	{
		while(j<COLS)
		{
			if(m_cannotAttackBoard[i][j] == 'X') 
			{
				//set cannot attack this spot again
				m_cannotAttackBoard[i][j] = 'V';
				//set the next index to search from
				if(j == COLS-1)
				{
					m_currentAttack.first = i + 1;
					m_currentAttack.second = 0;
				}
				else{
					m_currentAttack.first = i;
					m_currentAttack.second = j+1;
				}
				return pair<int, int>{m_currentAttack.first, m_currentAttack.second};
			}
			j++;
		}
		i++;
	}

	//end of attacks
	m_attacksDone = true;
	return{ 0,0 };
}

/*
this function is called at startup to update each players board game
*/
void BattleshipGameAlgo::setBoard(int player, const char** board, int numRows, int numCols) 
{
	//setting current attack to (0,0)
	m_currentAttack.first = 0;
	m_currentAttack.second = 0;
	m_myPlayerNum = player;
	m_board = GameBoardUtils::InitializeNewEmptyBoard();
	m_cannotAttackBoard = GameBoardUtils::InitializeNewEmptyBoard();
	for(int i=0;i<ROWS;i++)
	{
		for (int j=0;j<COLS;j++)
		{
			m_cannotAttackBoard[i][j] = 'X'; //X means we can attack here. 'V' means not
		}
	}
	GameBoardUtils::CloneBoardToPlayer(board, m_myPlayerNum, m_board);
	//prepering a matrix to know where not to attack
	GameBoardUtils::MarkCannotAttack(m_cannotAttackBoard, m_myPlayerNum, m_board);
}

/*
this function only updates board of player & adds score if needed.
checking of if the game is ended will be at main function
*/
void BattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) 
{
	//TODO: complete notify - is it needed at all?
}

BattleshipGameAlgo::~BattleshipGameAlgo()
{
	GameBoardUtils::DeleteBoard(m_board);
	GameBoardUtils::DeleteBoard(m_cannotAttackBoard);
}



