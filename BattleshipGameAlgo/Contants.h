#pragma once

#define TooManyShipsA "Too many ships for player A"
#define TooFewShipsA "Too few ships for player A"
#define TooManyShipsB "Too many ships for player B"
#define TooFewShipsB "Too few ships for player B"
#define AdjacentERR "Adjacent Ships on Board"

#define ROWS 10
#define COLS 10
#define BLANK '*'

#define LEGAL_NUMBER_OF_SHIPS_PER_PLAYER 5


enum class BoardFileErrorCode {Success, UnknownError};

// A Player boat types representation 
#define PlayerAID 0
#define RubberBoatA 'B'
#define RocketShipA 'P'
#define SubmarineA 'M'
#define DestroyerA 'D'

// B Player boat types representation 
#define PlayerBID 1
#define RubberBoatB 'b'
#define RocketShipB 'p'
#define SubmarineB 'm'
#define DestroyerB 'd'

// Ships wights
#define RubberBoatW 1
#define RocketShipW 2
#define SubmarineW 3
#define DestroyerW 4

// Sink Points 
#define RubberBoatPoints 2
#define RocketShipPoints 3
#define SubmarinePoints 7
#define DestroyerPoints 8

