#include "Bonus.h"
#include <thread>
#include "../Common/GameBoardUtils.h"

Bonus::Bonus(bool enable, int waitTimeout): enable(enable), waitInMiliseconds(waitTimeout), absX(0), absY(0)
{
}

void Bonus::Init(char** board, int rows, int cols)
{
	if (!enable)
		return;
	StoreCurrentConsoleState();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char current = board[i][j];
			PrintPlayerChar(current, j, i, 3, false);
		}
		cout << endl;
	}
	cout << endl;
	StoreCurrentCoord();
	Wait();
}

void Bonus::Wait()
{
	this_thread::sleep_for(std::chrono::milliseconds(waitInMiliseconds));
}

void* Bonus::GetstdOutHandle() 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
	{
		AppLogger.logFile << "[gotoxy] Failed to get std handle" << endl;
		return nullptr;
	}
	return handle;
}

// Remember how things were when we started
RetVal Bonus::StoreCurrentConsoleState()
{
	HANDLE hstdout = GetstdOutHandle();
	if(hstdout == nullptr)
	{
		//TODO: Missing
		AppLogger.logFile << "Failed to store current console state" << endl;
		return RetVal::UnknownError;
	}
	int retu = GetConsoleScreenBufferInfo(hstdout, &csbi);
	
	if(retu == 0)
	{
		AppLogger.logFile << "Failed to GetConsoleScreenBufferInfo";
		AppLogger.logFile << "Error Code " + GetLastError() << endl;
		AppLogger.logFile << "Failed to store current console state" << endl;
		return RetVal::UnknownError;
	}

	absX = csbi.dwCursorPosition.X;
	absY = csbi.dwCursorPosition.Y;
	return RetVal::Success;
}

RetVal Bonus::gotoxy(int x, int y, bool isAbsVal)
{
	COORD coord;
	coord.X = isAbsVal ? x : absX + x;
	coord.Y = isAbsVal ? y : absY + y;
	HANDLE handle = GetstdOutHandle();

	if (handle == nullptr)
	{
		return RetVal::UnknownError;
	}

	BOOL ret = SetConsoleCursorPosition(handle, coord);
	if (ret == 0)
	{
		AppLogger.logFile << "[gotoxy] Failed to SetConsoleCursorPosition" << endl;
		return RetVal::UnknownError;
	}
	return RetVal::Success;
}

RetVal Bonus::SetTextColor(int playerID)
{
	WORD color;

	if (playerID == PlayerAID)
		color = firstPlayerColor;
	else if (playerID == PlayerBID)
		color = secondPlayerColor;
	else
		color = blankPlayerColor;

	HANDLE handle = GetstdOutHandle();
	if (handle == nullptr)
	{
		return RetVal::UnknownError;
	}

	int retu = SetConsoleTextAttribute(handle, color);
	if (retu == 0)
	{
		AppLogger.logFile << "Failed to SetConsoleTextAttribute";
		AppLogger.logFile << "Error Code " + GetLastError() << endl;
		return RetVal::UnknownError;
	}
	return RetVal::Success;
}

RetVal Bonus::RestoreConsoleState()
{
	HANDLE hstdout = GetstdOutHandle();
	if (hstdout == nullptr)
	{
		//TODO: Missing
		AppLogger.logFile << "Failed to store current console state" << endl;
		return RetVal::UnknownError;
	}

	int retu = SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	if (retu == 0)
	{
		AppLogger.logFile << "Failed to SetConsoleTextAttribute";
		AppLogger.logFile << "Error Code " + GetLastError() << endl;
		return RetVal::UnknownError;
	}

	gotoxy(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y, true);
	return RetVal::Success;
}

RetVal Bonus::StoreCurrentCoord()
{
	CONSOLE_SCREEN_BUFFER_INFO buf;
	HANDLE hstdout = GetstdOutHandle();
	if (hstdout == nullptr)
	{
		//TODO: Missing
		AppLogger.logFile << "Failed to store current console state" << endl;
		return RetVal::UnknownError;
	}
	int retu = GetConsoleScreenBufferInfo(hstdout, &buf);

	if (retu == 0)
	{
		AppLogger.logFile << "Failed to GetConsoleScreenBufferInfo";
		AppLogger.logFile << "Error Code " + GetLastError() << endl;
		AppLogger.logFile << "Failed to store current coordinated" << endl;
		return RetVal::UnknownError;
	}

	csbi.dwCursorPosition.X = buf.dwCursorPosition.X;
	csbi.dwCursorPosition.Y = buf.dwCursorPosition.Y;
	return RetVal::Success;
}

void Bonus::PrintPlayerChar(char letter, short x, short y, int playerID, bool isWaitAfterPrint)
{
	if (!enable)
		return;

	int currentPlayer = playerID;
	if(currentPlayer == 3)
	{
		currentPlayer = GameBoardUtils::GetCharPlayerId(letter);
	}

	RetVal val = gotoxy(x, y);
	if(val != RetVal::Success)
	{
		AppLogger.logFile << "Failed to PrintPlayerChar due to error in gotoxy";
		return;
	}

	val = SetTextColor(currentPlayer);
	if (val != RetVal::Success)
	{
		AppLogger.logFile << "Failed to SetTextColor due to error in gotoxy";
		return;
	}

	printf("%c", letter);

	if (isWaitAfterPrint)
		Wait();
}

Bonus::~Bonus()
{
	if (!enable)
		return;

	RetVal ret = RestoreConsoleState();
	if (ret != RetVal::Success)
	{
		AppLogger.logFile << "Failed to RestoreConsoleState";
	}
}