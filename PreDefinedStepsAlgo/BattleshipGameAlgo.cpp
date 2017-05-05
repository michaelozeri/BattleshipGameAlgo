// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "BattleshipGameAlgo.h"
#include <utility>
#include "../Common/GameBoardUtils.h"
#include "../Common/AttackReciever.h"


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
void BattleshipGameAlgo::setBoard(int player, const char** board, int numRows, int numCols) 
{
	m_board = GameBoardUtils::InitializeNewEmptyBoard();
	GameBoardUtils::CloneBoardToPlayer(board, m_myPlayerNum, m_board);
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
	GameBoardUtils::DeleteBoard(m_board);
	delete m_attackReceiver;
}

//getter for if the attacks are finished
bool BattleshipGameAlgo::AttacksDone() const
{
	return m_attacksDone;
}

