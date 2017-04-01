#pragma once

#define ERR1A "Too many ships for player A"
#define ERR2A "Too few ships for player A"
#define ERR1B "Too many ships for player B"
#define ERR2B "Too few ships for player B"
#define ERR "Adjacent Ships on Board"
#define ROWS 10
#define COLS 10
#define BLANK '*'

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