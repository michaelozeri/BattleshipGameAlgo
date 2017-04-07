#include "stdafx.h"
#include "Contants.h"
#include "BattleshipGameAlgo.h"
#include "IOLib.h"
#include <algorithm>
#include "main.h" //TODO: remove?

/*script to find path of files taken from: 
http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c 
*/


using namespace std;

Logger AppLogger;
/*
 * @param argc 
 * @param argv 
 * @return path to .sboard file (non-default / "" in case the working directory is chosen )
 * @return "ERR" in case of error / file not found
 */
string GetBoardFilePath(int argc, char** argv)
{
	string nondefaultpath;
	string systemcallcommand;
	string templine;
	if (argc > 1) {
		nondefaultpath = argv[1];
		systemcallcommand = "dir " + nondefaultpath + " /b /a-d > file_names.txt";
		system(systemcallcommand.c_str());
		ifstream nondefaultstream("file_names.txt");
		while (true) {
			getline(nondefaultstream, templine);
			if (nondefaultstream.eof()) {
				cout << ".sboard file was not found in path given" << endl;
				break;
			}
			std::string delimiter = ".";
			size_t pos = 0;
			std::string token;
			while ((pos = templine.find(delimiter)) != std::string::npos) {
				token = templine.substr(0, pos);
				if (strcmp(token.c_str(), "sboard")) {
					cout << ".sboard file was found in path given" << endl;
					nondefaultstream.close();
					return nondefaultpath; //if we found a file with suffix .sboard then return the non-default file path
				}
				templine.erase(0, pos + delimiter.length());
			}
		}
		nondefaultstream.close();
	}
	//file not found in non-default directory
	systemcallcommand = "dir /b /a-d > file_names_working.txt";
	system(systemcallcommand.c_str());
	ifstream defaultpathstream("file_names_working.txt");
	while (true) {
		getline(defaultpathstream, templine);
		if (defaultpathstream.eof()) {
			cout << ".sboard file was not found at all!" << endl;
			defaultpathstream.close();
			return "ERR";
		}
		std::string delimiter = ".";
		size_t pos = 0;
		std::string token;
		while ((pos = templine.find(delimiter)) != std::string::npos) {
			token = templine.substr(0, pos);
			if (strcmp(token.c_str(), "sboard")) {
				cout << ".sboard file was found in working directory" << endl;
				defaultpathstream.close();
				return ""; //if we found a file with suffix .sboard then return the working directory file path
			}
			templine.erase(0, pos + delimiter.length());
		}
	}
}

char** ClonePlayerBoard(const char** fullBoard, int i)
{
	char** playerBoard = GameBordUtils::AllocateNewBoard();
	GameBordUtils::CloneBoardToPlayer(fullBoard, i, playerBoard);
	return playerBoard;
}

void SetPlayerBoards(char** board, string path, BattleshipGameAlgo& playerA, BattleshipGameAlgo& playerB)
{
	char** playerAboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerAID);
	AppLogger.logFile << "CloneBoardForA" << endl;
	GameBordUtils::PrintBoard(AppLogger.logFile, playerAboard, ROWS, COLS);

	char** playerBboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerBID);
	AppLogger.logFile << "CloneBoardForB" << endl;
	GameBordUtils::PrintBoard(AppLogger.logFile, playerBboard, ROWS, COLS);

	playerA.setBoard(const_cast<const char**>(playerAboard), ROWS, COLS);
	playerB.setBoard(const_cast<const char**>(playerBboard), ROWS, COLS);

	GameBordUtils::DeleteBoard(playerAboard);
	GameBordUtils::DeleteBoard(playerBboard);
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

	string boardPath = GetBoardFilePath(argc, argv); //TODO: finish attaching file name not only path
	if (boardPath == "ERR") {
		cout << "ERROR occured while getting board path" << endl;
		return -1;
	}

	// board - will save updated and full board of two players
	char** maingameboard = GameBordUtils::AllocateNewBoard();
	
	if(GameBordUtils::LoadBoardFromFile(maingameboard, ROWS, COLS, boardPath)!= BoardFileErrorCode::Success)
	{
		return -1;
	}
	GameBordUtils::PrintBoard(AppLogger.logFile, maingameboard, ROWS, COLS);

	string path = ""; //path to enter the non-default path to files, set to nullptr at default

	// Init players Instances
	BattleshipGameAlgo playerA("b.txt", PlayerAID);
	BattleshipGameAlgo playerB("c.txt", PlayerBID);
	SetPlayerBoards(maingameboard, path, playerA, playerB);
	
	//TODO: maybe change this inside BattleShipGameAlgo class instead of outside
	ShipDetatilsBoard playerAboardDetails(maingameboard, PlayerAID);
	ShipDetatilsBoard playerBboardDetails(maingameboard, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	//main game play

	// While not both of players ended their attacks
	while (!playerA.AttacksDone() && !playerB.AttacksDone())
	{
		pair<int, int> tempPair = GetNextPlayerAttack(playerIdToPlayNext, playerA, playerB);
		if ((tempPair.first == -1) && (tempPair.second == -1))
		{
			//TODO: Error occured during reading from file. Dispose all and exit gracefully - use dtor using delete
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

	//TODO - delete mainboard, dtor for shipdetails & gameutils?
	GameBordUtils::DeleteBoard(maingameboard);
	AppLogger.LoggerDispose();
	return 0;
}
