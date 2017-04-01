#include "stdafx.h"
#include "Contants.h"
#include "BattleshipGameAlgo.h"
#include "IOLib.h"
#include <algorithm>
#include "main.h" //TODO: remove?


using namespace std;

Logger AppLogger;
/**
 * \brief 
 * \param argc 
 * \param argv 
 * \return "" in case error eccured
 * TODO: Missing implemenation 
 */
string GetBoardFilePath(int argc, char** argv)
{
	//TODO
	//find *.sboard file at path.
	//std::ifstream mainboard( path + "board.sboard"); //TODO: check if we can guess the filename or try open all file with extension
	//if (!mainboard) {
	//	std::cout << "didnt manage to open board on path given, trying on working directory" << std::endl;
	//	mainboard.open("*.sboard");
	//}
	return "a.txt";
	//return "";
}

char** ClonePlayerBoard(const char** fullBoard, int i, GameBordUtils& gameBoardUtils)
{
	char** playerBoard = gameBoardUtils.GetNewBoard();
	gameBoardUtils.CloneBoardToPlayer(fullBoard, i, playerBoard);
	return playerBoard;
}

void SetPlayerBoards(GameBordUtils& gameBoardUtils, char** board, string path, BattleshipGameAlgo& playerA, BattleshipGameAlgo& playerB)
{
	char** playerAboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerAID, gameBoardUtils);
	AppLogger.logFile << "CloneBoardForA" << endl;
	gameBoardUtils.PrintBoard(AppLogger.logFile, playerAboard, ROWS, COLS);

	char** playerBboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerBID, gameBoardUtils);
	AppLogger.logFile << "CloneBoardForB" << endl;
	gameBoardUtils.PrintBoard(AppLogger.logFile, playerBboard, ROWS, COLS);

	playerA.setBoard(const_cast<const char**>(playerAboard), ROWS, COLS);
	playerB.setBoard(const_cast<const char**>(playerBboard), ROWS, COLS);

	gameBoardUtils.DeleteBoard(playerAboard);
	gameBoardUtils.DeleteBoard(playerBboard);
}

void InitLogger()
{
	AppLogger.InitLogger("Battle.log");
}

pair<int, int> GetNextPlayerAttack(int player_id,  BattleshipGameAlgo& player_a,  BattleshipGameAlgo& player_b)
{
	if (player_id == PlayerAID)
	{
		return player_a.attack();
	}
	if (player_id == PlayerBID)
		return player_b.attack();

	// Fatal Error
	AppLogger.logFile << "Fatal error occured. Attacok move was asked for non exixting player id " << player_id << endl;
	return{ -1,-1 };
}

AttackResult GetAttackResult(const pair<int, int>& pair, int current_player, ShipDetatilsBoard& detailsA, ShipDetatilsBoard& detailsB)
{
	return (current_player == PlayerAID) ? detailsB.GetAttackResult(pair) : detailsA.GetAttackResult(pair);
}

bool IsPlayerWon(int currentPlayer, ShipDetatilsBoard& detailsA, ShipDetatilsBoard& detailsB)
{
	return currentPlayer == PlayerAID ? detailsB.IsLoose() : detailsA.IsLoose();
}

void PrintPoints(BattleshipGameAlgo& playerA, BattleshipGameAlgo& playerB)
{
	cout << "Points:" << endl;
	cout << "Player A: " << playerA.GetSctore() << endl;
	cout << "Player B: " << playerB.GetSctore() << endl;
}

int main(int argc, char* argv[]) 
{
	InitLogger();
	GameBordUtils gameBoardUtils;

	string boardPath = GetBoardFilePath(argc, argv);
	if (boardPath == "")
		return -1;

	// board - will save updated and full board of two players
	char** board = gameBoardUtils.GetNewBoard();
	
	if(gameBoardUtils.LoadBoardFromFile(board, ROWS, COLS, boardPath)!= BoardFileErrorCode::Success)
	{
		return -1;
	}
	gameBoardUtils.PrintBoard(AppLogger.logFile, board, ROWS, COLS);

	string path = ""; //path to enter the non-default path to files, set to nullptr at default

	// Init players Instances
	BattleshipGameAlgo playerA("b.txt", PlayerAID);
	BattleshipGameAlgo playerB("c.txt", PlayerBID);
	SetPlayerBoards(gameBoardUtils, board, path, playerA, playerB);
	
	ShipDetatilsBoard playerAboardDetails(board, PlayerAID);
	ShipDetatilsBoard playerBboardDetails(board, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	//main game play

	// While not both of players ended their attacks
	while (!playerA.FinishedAttacks() && !playerB.FinishedAttacks())
	{
		pair<int, int> tempPair = GetNextPlayerAttack(playerIdToPlayNext, playerA, playerB);
		if ((tempPair.first == -1) && (tempPair.second == -1))
		{
			//TODO: Error occured during reading from file Dispose all and exit gracefully
			return -1;
		}

		if ((tempPair.first == 0) && (tempPair.second == 0))
		{
			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			continue;
		}
		else
		{
			//calculate attack and update board
			AttackResult tempattackresult = GetAttackResult(tempPair, playerIdToPlayNext, playerAboardDetails, playerBboardDetails);
			
			//update players
			playerA.notifyOnAttackResult(PlayerAID, tempPair.first, tempPair.second, tempattackresult);
			playerB.notifyOnAttackResult(PlayerBID, tempPair.first, tempPair.second, tempattackresult);

			if (tempattackresult == AttackResult::Miss)
			{
				// Flip Players
				playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			}
			else
			{
				if(IsPlayerWon(playerIdToPlayNext,playerAboardDetails, playerBboardDetails))
				{
					char wonChar = (playerIdToPlayNext == PlayerAID) ? 'A' : 'B';
					cout << "Player " << wonChar << " won" << endl;
					PrintPoints(playerA, playerB);
					 
					// TODO - Dispose all resources
					return 0;
				}
			}
		}
	}

	// TODO - Dispose all resources
	PrintPoints(playerA, playerB);

	gameBoardUtils.DeleteBoard(board);
	AppLogger.LoggerDispose();
	return 0;
}
