//
//  Constants.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef Tetrominos_Constants_h
#define Tetrominos_Constants_h

static const int GRID_WIDTH = 10;
static const int GRID_HEIGHT = 20;
static const float INITIAL_STEP_INTERVAL = 1.0f;
static const float DROP_VELOCITY = 4000.0f;
static const float HOLD_VELOCITY = -4000.0f;
static const int BLOCKS_IN_TETROMINO = 4;
static const int SCORE_TO_ACCELERATE = 5;
static const float ACCELERATION_FACTOR = 0.8f;
static const float SPEED_MAX = 0.1f;
static const float TIME_PER_GAME = 120.0f;
static const int LINE_PER_GAME = 40;

static const char* USERDEFAULT_KEY_HIGH_SCORE = "2min hightscore";
static const char* USERDEFAULT_KEY_TIME_HIGH_SCORE = "40line hightscore";

enum class TetrominoType
{
    NONE = -1,
    I = 0,
    O = 1,
    T = 2,
    S = 3,
    Z = 4,
    J = 5,
    L = 6
};

enum class GameMode
{
    LINE40_MODE,
    MIN2_MODE
};

#endif
