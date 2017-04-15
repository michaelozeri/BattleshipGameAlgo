#include "stdafx.h"
#include "Contants.h"
#include "BattleshipGameAlgo.h"
#include "IOLib.h"
#include <algorithm>
#include <windows.h>
#include <thread>
#include "Bonus.h"

using namespace std;

/*
 * @param argc - of main program 
 * @param argv - of main program
 * @param filesuffix - the suffix of the file we are searching for
 * @return path to .sboard file (non-default / "" in case the working directory is chosen )
 * @return "ERR" in case of error / file not found
 */
string GetFilePathBySuffix(int argc, char** argv,string filesuffix)
{
	char currentdirectory[_MAX_PATH];
	//reference: taken from : http://stackoverflow.com/questions/19691058/c-visual-studio-current-working-directory
	_fullpath(currentdirectory, ".\\", _MAX_PATH); // obtain current directory
	string filename,suffix;
	string delimiter = ".";
	string nondefaultpath;
	string systemcallcommand;
	size_t pos = 0;
	string templine;
	if (argc > 1) {
		nondefaultpath = argv[1];
		systemcallcommand = "dir " + nondefaultpath + " /b /a-d > file_names.txt";
		system(systemcallcommand.c_str());
		ifstream nondefaultstream("file_names.txt");
		while (true) {
			getline(nondefaultstream, templine);
			if (nondefaultstream.eof()) {
				if (filesuffix == ".sboard") {
					cout << "Missing board file (*.sboard) looking in path: " << currentdirectory << endl;
				}
				else if (filesuffix == ".attack-a") {
					cout << "Missing attack file for player A(*.attack - a) looking in path: " << currentdirectory << endl;
				}
				else {
					cout << "Missing attack file for player B(*.attack - b) looking in path: " << currentdirectory << endl;
				}
				break;
			}
			pos = templine.find(delimiter);
			suffix = templine.substr(pos, templine.length());
			filename = templine.substr(0, pos);
			if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
				nondefaultstream.close();
				if (nondefaultpath.at(nondefaultpath.length() - 1) == '\\') {
					return nondefaultpath + filename + suffix; //argument is inserted with '\' at string ending
				}
				else {
					return nondefaultpath + '\\' + filename + suffix; //argument is inserted without '\' at ending
				}
			}
		}
		nondefaultstream.close();
	}
	pos = 0;
	//file not found in non-default directory
	systemcallcommand = "dir /b /a-d > file_names_working.txt";
	system(systemcallcommand.c_str());
	ifstream defaultpathstream("file_names_working.txt");
	while (true) {
		getline(defaultpathstream, templine);
		if (defaultpathstream.eof()) {
			cout << filesuffix+ " file was not found in working directory" << endl;
			defaultpathstream.close();
			return "ERR";
		}
		pos = templine.find(delimiter);
		suffix = templine.substr(pos, templine.length());
		filename = templine.substr(0, pos);
		if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
			defaultpathstream.close();
			return filename + suffix;
		}
	}
	defaultpathstream.close();
}

char** ClonePlayerBoard(const char** fullBoard, int i)
{
	char** playerBoard = GameBordUtils::AllocateNewBoard();
	GameBordUtils::CloneBoardToPlayer(fullBoard, i, playerBoard);
	return playerBoard;
}

void SetPlayerBoards(char** board, BattleshipGameAlgo& playerA, BattleshipGameAlgo& playerB)
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

AttackResult GetAttackResult(const pair<int, int>& pair, char** board, ShipDetatilsBoard* detailsA, ShipDetatilsBoard* detailsB)
{
	return GameBordUtils::IsPlayerIdChar(PlayerAID,board[pair.first][pair.second]) ? detailsA->GetAttackResult(pair) : detailsB->GetAttackResult(pair);
}

bool IsPlayerWon(int currentPlayer, ShipDetatilsBoard* detailsA, ShipDetatilsBoard* detailsB)
{
	return currentPlayer == PlayerAID ? detailsB->IsLoose() : detailsA->IsLoose();
}

void PrintPoints(ShipDetatilsBoard* playerA, ShipDetatilsBoard* playerB)
{
	cout << "Points:" << endl;
	cout << "Player A: " << playerB->negativeScore << endl;
	cout << "Player B: " << playerA->negativeScore << endl;
}

void ChangeFontSize()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX orig = { sizeof(orig) };

	if (GetCurrentConsoleFontEx(hStdout, FALSE, &orig))
		AppLogger.logFile << "Got\n";
	else
		AppLogger.logFile << GetLastError();
	
	orig.dwFontSize.X = 12;
	orig.dwFontSize.Y = 16;

	if (SetCurrentConsoleFontEx(hStdout, FALSE, &orig))
		AppLogger.logFile << "Set\n";
	else
		AppLogger.logFile << endl << GetLastError();

}

void PrintSinkCharRec(char** maingameboard,Bonus* b , int i, int j, int player)
{
	if (i < 0 || i >= ROWS || j < 0 || j >= COLS) // Stop recursion condition
	{
		return;
	}

	char currentCell = maingameboard[i][j];
	if (currentCell != HIT_CHAR)
		return;

	maingameboard[i][j] = SINK_CHAR;
	b->PrintPlayerChar(maingameboard[i][j], j, i, player);
	PrintSinkCharRec(maingameboard,b, i, j - 1, player);
	PrintSinkCharRec(maingameboard,b, i, j + 1, player);
	PrintSinkCharRec(maingameboard,b, i - 1, j, player);
	PrintSinkCharRec(maingameboard,b, i + 1, j, player);
}

int main(int argc, char* argv[]) 
{
	//TODO: OR - remove stdafx.cpp and all refernce to it
	//TODO: Check if we need to print something else to the console
	InitLogger();
                   
	ChangeFontSize();
	BonusParams p; //TODO OR- Init this struct with parameters from command line. defaults set

	string mainboardpath = GetFilePathBySuffix(argc, argv,".sboard"); 
	if (mainboardpath == "ERR") {
		cout << "ERROR occured while getting board path" << endl;
		AppLogger.LoggerDispose();
		return -1;
	}

	// board - will save updated and full board of two players
	char** maingameboard = GameBordUtils::AllocateNewBoard();
	
	if(GameBordUtils::LoadBoardFromFile(maingameboard, ROWS, COLS, mainboardpath)!= BoardFileErrorCode::Success)
	{
		GameBordUtils::DeleteBoard(maingameboard);
		AppLogger.LoggerDispose();
		return -1;
	}
	GameBordUtils::PrintBoard(AppLogger.logFile, maingameboard, ROWS, COLS);

	string Aattackpath, Battackpath;
	Aattackpath = GetFilePathBySuffix(argc, argv, ".attack-a");
	if (Aattackpath == "ERR") 
	{
		cout << "ERROR in retrieving attack file of player A" << endl;
		GameBordUtils::DeleteBoard(maingameboard);
		AppLogger.LoggerDispose();
		return -1;
	}
	Battackpath = GetFilePathBySuffix(argc, argv, ".attack-b");
	if (Battackpath == "ERR") 
	{
		cout << "ERROR in retrieving attack file of player B" << endl;
		GameBordUtils::DeleteBoard(maingameboard);
		AppLogger.LoggerDispose();
		return -1;
	}

	// Init players Instances
	BattleshipGameAlgo playerA(Aattackpath, PlayerAID);
	BattleshipGameAlgo playerB(Battackpath, PlayerBID);
	SetPlayerBoards(maingameboard, playerA, playerB);
	
	//TODO: maybe change this inside BattleShipGameAlgo class instead of outside - (In next HW Next)
	ShipDetatilsBoard* playerAboardDetails = new ShipDetatilsBoard(maingameboard, PlayerAID);
	ShipDetatilsBoard* playerBboardDetails = new ShipDetatilsBoard(maingameboard, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	Bonus* bonus = new Bonus(!p.isQuiet, p.delayInMiliseconds);
	bonus->Init(maingameboard, ROWS, COLS);
	//main game play

	// While not both of players ended their attacks
	while (!playerA.AttacksDone() || !playerB.AttacksDone())
	{
		pair<int, int> tempPair = GetNextPlayerAttack(playerIdToPlayNext, playerA, playerB);

		//aligned both axis -1 because main board starts from (0,0)
		tempPair = { tempPair.first - 1,tempPair.second - 1 };

		if ((tempPair.first == -2) && (tempPair.second == -2))
		{
			GameBordUtils::DeleteBoard(maingameboard);
			delete playerAboardDetails;
			delete playerBboardDetails;
			AppLogger.LoggerDispose();
			return -1;
		}

		if ((tempPair.first == -1) && (tempPair.second == -1))
		{
			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			continue;
		}
		else
		{
			char attckCell = maingameboard[tempPair.first][tempPair.second];
			bool isSelfAttack = GameBordUtils::IsPlayerIdChar(playerIdToPlayNext, attckCell);

			//calculate attack and update mainboard
			AttackResult tempattackresult = GetAttackResult(tempPair, maingameboard, playerAboardDetails, playerBboardDetails);

			//update players
			playerA.notifyOnAttackResult(playerIdToPlayNext, tempPair.first, tempPair.second, tempattackresult);
			playerB.notifyOnAttackResult(playerIdToPlayNext, tempPair.first, tempPair.second, tempattackresult);

			if (tempattackresult != AttackResult::Miss)
			{
				int playerTosetColor;
				if(isSelfAttack)
				{
					playerTosetColor = playerIdToPlayNext;
				}
				else
				{
					playerTosetColor = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
				}

				if(tempattackresult == AttackResult::Sink)
				{
					// In case sink update all the cell to SINK_CHAR
					PrintSinkCharRec(maingameboard,bonus, tempPair.first, tempPair.second, playerTosetColor);
				}
				else // In case hit update only the target cell
					bonus->PrintPlayerChar(maingameboard[tempPair.first][tempPair.second], tempPair.second, tempPair.first, playerTosetColor);
			}

			if (tempattackresult == AttackResult::Miss || isSelfAttack)
			{
				// Flip Players
				playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			}

			if (IsPlayerWon(PlayerAID, playerAboardDetails, playerBboardDetails))
			{
				char wonChar = 'A';
				delete bonus;
				cout << "Player " << wonChar << " won" << endl;
				PrintPoints(playerAboardDetails, playerBboardDetails);

				GameBordUtils::DeleteBoard(maingameboard);
				delete playerAboardDetails;
				delete playerBboardDetails;
				return 0;
			}
			if (IsPlayerWon(PlayerBID, playerAboardDetails, playerBboardDetails))
			{
				char wonChar = 'B';
				cout << "Player " << wonChar << " won" << endl;
				delete bonus;
				PrintPoints(playerAboardDetails, playerBboardDetails);

				GameBordUtils::DeleteBoard(maingameboard);
				delete playerAboardDetails;
				delete playerBboardDetails;
				return 0;
			}
		}
	}

	delete bonus;
	PrintPoints(playerAboardDetails, playerBboardDetails);

	GameBordUtils::DeleteBoard(maingameboard);
	delete playerAboardDetails;
	delete playerBboardDetails;	
	AppLogger.LoggerDispose();
	return 0;
}
