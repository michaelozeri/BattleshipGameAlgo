#include "ShipDetailsBoard.h"

ShipDetailsBoard::ShipDetailsBoard(char** board, int playerID) : playerID(playerID), mainboard(board), RubberBoatCells(0), RocketShipCells(0), SubmarineCells(0), DestroyeCells(0), negativeScore(0)
{
	for (size_t i = 0; i < ROWS; i++)
	{
		for (size_t j = 0; j < COLS; j++)
		{
			char cell = board[i][j];
			if (_utils.IsPlayerIdChar(playerID, cell))
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
AttackResult ShipDetailsBoard::GetAttackResult(pair<int, int> attack)
{
	AttackResult result = AttackResult::Miss;
	char cell = mainboard[attack.first][attack.second];
	if (_utils.IsPlayerIdChar(playerID, cell))
	{
		mainboard[attack.first][attack.second] = HIT_CHAR;
		switch (tolower(cell))
		{
		case 'b':
			RubberBoatCells--;
			result = RubberBoatCells % RubberBoatW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? RubberBoatPoints : 0;
			break;
		case 'p':
			RocketShipCells--;
			result = RocketShipCells % RocketShipW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? RocketShipPoints : 0;
			break;
		case 'm':
			SubmarineCells--;
			result = SubmarineCells % SubmarineW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? SubmarinePoints : 0;
			break;
		case 'd':
			DestroyeCells--;
			result = DestroyeCells % DestroyerW == 0 ? AttackResult::Sink : AttackResult::Hit;
			negativeScore += result == AttackResult::Sink ? DestroyerPoints : 0;
			break;
		default:
			// Restore the previous value - should not get here
			mainboard[attack.first][attack.second] = cell;
			break;
		}
	}
	return result;
}

bool ShipDetailsBoard::IsLoose() const
{
	int sum = RubberBoatCells + RocketShipCells + SubmarineCells + DestroyeCells;
	return (sum == 0);
}