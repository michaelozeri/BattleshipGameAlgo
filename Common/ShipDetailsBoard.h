#pragma once
#include "GameBoardUtils.h"
#include "IBattleshipGameAlgo.h"

/*
* a utility class for keeping track of game state
*/
class ShipDetailsBoard
{
	GameBoardUtils _utils;
public:
	int playerID;
	char** mainboard;

	int RubberBoatCells;
	int RocketShipCells;
	int SubmarineCells;
	int DestroyeCells;

	int negativeScore; // Save the score of the other player
	ShipDetailsBoard(char** board, int playerID);
	AttackResult GetAttackResult(pair<int, int> attack);
	bool IsLoose() const;
};