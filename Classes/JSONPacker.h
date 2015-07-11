//
//  JSONPacker.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef __Tetrominos__JSONPacker__
#define __Tetrominos__JSONPacker__

#include "Cocos2d.h"
#include "Coordinate.h"
#include "Constants.h"

namespace JSONPacker
{
    struct GameState
    {
        std::string name;
        bool gameOver;
        int score;
        std::vector<std::vector<cocos2d::Color3B>> board;
    };

    struct TetrominoState
    {
        cocos2d::Color3B color;
        std::vector<std::vector<Coordinate>> rotations;
    };
    
    GameState unpackGameStateJSON(std::string json);
    std::string packGameState(const GameState data);

    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type);
    std::string packTetrominoJSON(TetrominoState tetrominoState);
}

#endif /* defined(__Tetrominos__JSONPacker__) */
