#include <windows.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "../Common/GameBoardUtils.h"
#include "../Common/ShipDetailsBoard.h"
#include "../Common/Bonus.h"
#include <vector>
#include "IBattleshipGameAlgo.h"

typedef IBattleshipGameAlgo *(*GetAlgorithmFuncType)(); //TODO: is it right to declare here?

using namespace std;

int LoadDllFilesByOrder(int argc, char** argv,string dirPath,GetAlgorithmFuncType& playerA,GetAlgorithmFuncType& playerB,bool dirExists) {

	HANDLE dir;

	WIN32_FIND_DATAA fileData; //data struct for file
	vector<HINSTANCE>::iterator itr;

	// vector of shape objects we create
	vector<IBattleshipGameAlgo *> shape_vec;
	vector<IBattleshipGameAlgo *>::iterator aitr;

	// define function of the type we expect
	
	GetAlgorithmFuncType GetAlgorithmFunc;
	vector<tuple<string, HINSTANCE, GetAlgorithmFuncType>> dll_vec; // vector of <Shape Name, dll handle, GetShape function ptr>
	vector<tuple<string, HINSTANCE, GetAlgorithmFuncType>>::iterator vitr;
	vector<string> algoNames;

	// iterate over *.dll files in path
	string s = "\\*.dll"; // only .dll endings

	if (!dirExists) {
		char currentdirectory[_MAX_PATH];
		_fullpath(currentdirectory, ".\\", _MAX_PATH); // obtain current directory
		dirPath.assign(currentdirectory);
	}

	dir = FindFirstFileA((dirPath + s).c_str(), &fileData); 
	if (dir != INVALID_HANDLE_VALUE) //check if the dir opened successfully
	{
		// test each file suffix and set variables as needed
		do
		{
			string fileName = fileData.cFileName;
			string fullFileName = dirPath + "\\" + fileName;
			string AlgorithmName = fileName.substr(0, fileName.find("."));

			// Load dynamic library
			HINSTANCE hDll = LoadLibraryA(fullFileName.c_str()); // Notice: Unicode compatible version of LoadLibrary
			if (!hDll)
			{
				std::cout << "could not load the dynamic library" << std::endl;
				return EXIT_FAILURE;
			}

			// Get function pointer
			GetAlgorithmFunc = (GetAlgorithmFuncType)GetProcAddress(hDll, "GetAlgorithm");
			if (!GetAlgorithmFunc)
			{
				std::cout << "could not load function GetShape()" << std::endl;
				return EXIT_FAILURE;
			}

			//insert to the vector tuple (algoName,handler,pointer to func)
			dll_vec.push_back(make_tuple(AlgorithmName, hDll, GetAlgorithmFunc));
			algoNames.push_back(AlgorithmName);

		} while (FindNextFileA(dir, &fileData)); // Notice: Unicode compatible version of FindNextFile
	}
	else {
		cout << "ERROR: could not load dll files" << endl;
		return EXIT_FAILURE; //TODO: check if ok to exit like this
	}
	if (dll_vec.size() < 2) {
		cout << "ERROR: not enough dll files" << endl;
		return EXIT_FAILURE; //TODO: check is ok and we do not need to search working directory
	}

	//sort vector of algorithm names
	std::sort(algoNames.begin(), algoNames.end());
	string firstAlgoName = algoNames[0];
	string secondAlgoName = algoNames[0];
	
	//find first algo implemantation in dll_vec
	for (vitr=dll_vec.begin(); vitr != dll_vec.end(); ++vitr) {
		if (!firstAlgoName.compare(get<0>(*vitr))) {
			playerA = get<2>(*vitr);
		}
		if (!secondAlgoName.compare(get<0>(*vitr))) {
			playerB = get<2>(*vitr);
		}
	}
	return 0;
}

//taken from: http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
bool DirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

/*
 * @param argc - of main program 
 * @param argv - of main program
 * @param filesuffix - the suffix of the file we are searching for
 * @return path to .sboard file (non-default / "" in case the working directory is chosen )
 * @return "ERR" in case of error / file not found
 */
string GetFilePathBySuffix(int argc, string customPath ,string filesuffix, bool direxists = true)
{
	char currentdirectory[_MAX_PATH];
	//reference: taken from : http://stackoverflow.com/questions/19691058/c-visual-studio-current-working-directory
	_fullpath(currentdirectory, ".\\", _MAX_PATH); // obtain current directory
	string filename,suffix;
	string delimiter = ".";
	string nondefaultpath;
	string systemcallcommand;
	size_t pos;
	string templine;
	if (argc > 1){
		if (direxists) {
			nondefaultpath = customPath;
			systemcallcommand = "dir \"" + nondefaultpath + "\" /b /a-d > file_names.txt";
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
	}
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
}

char** ClonePlayerBoard(const char** fullBoard, int i)
{
	char** playerBoard = GameBoardUtils::InitializeNewEmptyBoard();
	GameBoardUtils::CloneBoardToPlayer(fullBoard, i, playerBoard);
	return playerBoard;
}

void SetPlayerBoards(char** board, IBattleshipGameAlgo* playerA, IBattleshipGameAlgo* playerB)
{
	char** playerAboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerAID);
	AppLogger.logFile << "CloneBoardForA" << endl;
	GameBoardUtils::PrintBoard(AppLogger.logFile, playerAboard, ROWS, COLS);

	char** playerBboard = ClonePlayerBoard(const_cast<const char**>(board), PlayerBID);
	AppLogger.logFile << "CloneBoardForB" << endl;
	GameBoardUtils::PrintBoard(AppLogger.logFile, playerBboard, ROWS, COLS);
	playerA->setBoard(0,const_cast<const char**>(playerAboard), ROWS, COLS);
	playerB->setBoard(1,const_cast<const char**>(playerBboard), ROWS, COLS);

	GameBoardUtils::DeleteBoard(playerAboard);
	GameBoardUtils::DeleteBoard(playerBboard);
}

void InitLogger()
{
	AppLogger.InitLogger("Battle.log");
}

pair<int, int> GetNextPlayerAttack(int player_id,  IBattleshipGameAlgo* player_a, IBattleshipGameAlgo* player_b)
{
	if (player_id == PlayerAID)
	{
		return player_a->attack();
	}
	if (player_id == PlayerBID) {
		return player_b->attack();
	}
	// Fatal Error
	AppLogger.logFile << "Fatal error occured. Attack move was asked for non exixting player id " << player_id << endl;
	return{ -1,-1 };
}

AttackResult GetAttackResult(const pair<int, int>& pair, char** board, ShipDetailsBoard* detailsA, ShipDetailsBoard* detailsB)
{
	return GameBoardUtils::IsPlayerIdChar(PlayerAID,board[pair.first][pair.second]) ? detailsA->GetAttackResult(pair) : detailsB->GetAttackResult(pair);
}

bool IsPlayerWon(int currentPlayer, ShipDetailsBoard* detailsA, ShipDetailsBoard* detailsB)
{
	return currentPlayer == PlayerAID ? detailsB->IsLoose() : detailsA->IsLoose();
}

void PrintPoints(ShipDetailsBoard* playerA, ShipDetailsBoard* playerB)
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
	GetAlgorithmFuncType getPlayerAAlgo, getPlayerBAlgo;

	bool dirExists = false; 
	bool AattacksDone = false;
	bool BattacksDone = false;
	InitLogger();
                   
	ChangeFontSize();
	BonusParams p; 
	string dirPath("NS");
	
	//aquiring arguments and checking if dir exists
	if (argc > 1) {
		for (int i = 1; i < argc; i++) 
		{
			string s = argv[i];
			if (!s.compare("-quiet")) {
				p.isQuiet = true;
			}
			else if (!s.compare("-delay")) 
			{
				int del = atoi(argv[i + 1]);
				p.delayInMiliseconds = del;
				i++;
			}
			else
			{
				dirPath = argv[i];
			}
		}
		if (dirPath.compare("NS") != 0)
		{
			dirExists = DirExists(dirPath);
			if (!dirExists) 
			{
				cout << "Wrong path:" << dirPath << endl;
			}
		}
	}

	//TODO: check this function is working + add search in working directory in case less then 2 were found
	if (LoadDllFilesByOrder(argc, argv, dirPath, getPlayerAAlgo, getPlayerBAlgo,dirExists)) {
		cout << "Error loading dll files. exiting" << endl;
		return -1; //TODO: release all and check is ok
	}

	//get .sboard file path
	string mainboardpath = GetFilePathBySuffix(argc, dirPath,".sboard",dirExists);
	if (mainboardpath == "ERR") {
		cout << "ERROR occured while getting board path" << endl;
		AppLogger.LoggerDispose();
		return -1;
	}

	// board - will save updated and full board of two players
	char** mainGameBoard = GameBoardUtils::InitializeNewEmptyBoard();
	//load main game board from file & validate the board
	if(GameBoardUtils::LoadBoardFromFile(mainGameBoard, ROWS, COLS, mainboardpath)!= BoardFileErrorCode::Success)
	{
		GameBoardUtils::DeleteBoard(mainGameBoard);
		AppLogger.LoggerDispose();
		return -1;
	}
	GameBoardUtils::PrintBoard(AppLogger.logFile, mainGameBoard, ROWS, COLS);

	//get attack files path
	string Aattackpath, Battackpath;
	Aattackpath = GetFilePathBySuffix(argc, dirPath, ".attack-a", dirExists);
	if (Aattackpath == "ERR") 
	{
		cout << "ERROR in retrieving attack file of player A" << endl;
		GameBoardUtils::DeleteBoard(mainGameBoard);
		AppLogger.LoggerDispose();
		return -1;
	}
	Battackpath = GetFilePathBySuffix(argc, dirPath, ".attack-b", dirExists);
	if (Battackpath == "ERR") 
	{
		cout << "ERROR in retrieving attack file of player B" << endl;
		GameBoardUtils::DeleteBoard(mainGameBoard);
		AppLogger.LoggerDispose();
		return -1;
	}

	// Init players Instances
	IBattleshipGameAlgo * algoA = getPlayerAAlgo();
	IBattleshipGameAlgo * algoB = getPlayerBAlgo();

	//setting player boards
	SetPlayerBoards(mainGameBoard, algoA, algoB);
	if (!(algoA->init(Aattackpath)) || !(algoB->init(Battackpath))) {
		cout << "ERROR: could not init attack files" << endl;
	}

	//TODO: maybe change this inside BattleShipGameAlgo class instead of outside - (In next HW Next)
	ShipDetailsBoard* playerAboardDetails = new ShipDetailsBoard(mainGameBoard, PlayerAID);
	ShipDetailsBoard* playerBboardDetails = new ShipDetailsBoard(mainGameBoard, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	Bonus* bonus = new Bonus(!p.isQuiet, p.delayInMiliseconds);
	bonus->Init(mainGameBoard, ROWS, COLS);
	//main game play

	// While not both of players ended their attacks - //TODO: make outsidebollean to set instead of attacks done
	while (!AattacksDone || !BattacksDone)
	{
		pair<int, int> tempPair = GetNextPlayerAttack(playerIdToPlayNext, algoA, algoB);

		//aligned both axis -1 because main board starts from (0,0) //TODO: check if this implementation with -1 for each axis is ok
		tempPair = { tempPair.first - 1,tempPair.second - 1 };

		//Error occurred 
		if ((tempPair.first == -2) && (tempPair.second == -2))
		{
			GameBoardUtils::DeleteBoard(mainGameBoard);
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
			char attckCell = mainGameBoard[tempPair.first][tempPair.second];
			bool isSelfAttack = GameBoardUtils::IsPlayerIdChar(playerIdToPlayNext, attckCell);

			//calculate attack and update mainboard
			AttackResult tempattackresult = GetAttackResult(tempPair, mainGameBoard, playerAboardDetails, playerBboardDetails);

			//update players
			algoA->notifyOnAttackResult(playerIdToPlayNext, tempPair.first, tempPair.second, tempattackresult);
			algoB->notifyOnAttackResult(playerIdToPlayNext, tempPair.first, tempPair.second, tempattackresult);

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
					PrintSinkCharRec(mainGameBoard,bonus, tempPair.first, tempPair.second, playerTosetColor);
				}
				else // In case hit update only the target cell
					bonus->PrintPlayerChar(mainGameBoard[tempPair.first][tempPair.second], tempPair.second, tempPair.first, playerTosetColor);
			}

			if (tempattackresult == AttackResult::Miss || isSelfAttack)
			{
				// Flip Players
				playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			}

			if (IsPlayerWon(PlayerAID, playerAboardDetails, playerBboardDetails))
			{
				delete bonus;
				cout << "Player A won" << endl;
				PrintPoints(playerAboardDetails, playerBboardDetails);

				GameBoardUtils::DeleteBoard(mainGameBoard);
				delete playerAboardDetails;
				delete playerBboardDetails;
				return 0;
			}
			if (IsPlayerWon(PlayerBID, playerAboardDetails, playerBboardDetails))
			{
				delete bonus;
				cout << "Player B won" << endl;
				PrintPoints(playerAboardDetails, playerBboardDetails);

				GameBoardUtils::DeleteBoard(mainGameBoard);
				delete playerAboardDetails;
				delete playerBboardDetails;
				return 0;
			}
		}
	}

	delete bonus;
	PrintPoints(playerAboardDetails, playerBboardDetails);

	GameBoardUtils::DeleteBoard(mainGameBoard);
	delete playerAboardDetails;
	delete playerBboardDetails;	
	AppLogger.LoggerDispose();
	return 0;
}
