// BattleshipGameAlgo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "BattleshipGameAlgo.h"

std::pair<int,int> BattleshipGameAlgo::attack() {

	std::string attackRow, attackCol;
	int row, col;

	std::getline(_attack,attackRow,',');//get the attack Row
	if (!_attack) {
		std::cout << "read from file failure!" << std::endl;
	}
	std::getline(_attack, attackCol,',');//get the attack Col
	if (!_attack) {
		if (_attack.eof()) {
			std::cout << "EOF of player: " << _myPlayerNum << std::endl;
			_attacksDone = true;
			std::pair<int, int> p1(0, 0);
			return p1;
		}
		else {
			std::cout << "read from file failure!" << std::endl;
			//TODO: handle this
		}
	}

	//convert attack string into int's
	row = std::stoi(attackRow);
	col = std::stoi(attackCol);

	//checking if ok
	if ((row > 10) || (row < 1) || (col < 1) || (col > 10)) {
		std::cout << "bad attack line!" << std::endl;
		std::pair<int, int> p1(0, 0);
		return p1;
	}
	std::cout << "attack taken from file is: " << row << "," << col << std::endl;

	std::pair<int, int> p1(row, col);
	return p1;
}


void BattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols) {
	_board[numRows][numCols]; //TODO: is this ok like this?
	int i, j;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			_board[i][j] = board[i][j];
		}
	}
	std::cout << "finished copying array" << std::endl;
}

//TODO
void BattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	char check = _board[row][col];
	if (_myPlayerNum == player) {
		
	}
	else {

	}

}

//default constructor
BattleshipGameAlgo::BattleshipGameAlgo() : _currentscore(0),  _myPlayerNum(0), _attacksDone(false) {
	std::cout << "default constructor" << std::endl;
}

//non - default constructor
BattleshipGameAlgo::BattleshipGameAlgo(const std::string & attackPath, const int playerNum) : _currentscore(0), _myPlayerNum(playerNum), _attacksDone(false) {
	_attack.open(attackPath);
	std::cout << "non - default constructor" << std::endl;
}

//getter for if the attacks are finished
bool BattleshipGameAlgo::FinishedAttacks() {
	return _attacksDone;
}
