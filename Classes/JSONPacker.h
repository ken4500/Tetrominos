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
    struct TetrominoState
    {
        cocos2d::Color3B color;
        std::vector<std::vector<Coordinate>> rotations;
    };
    
    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type);
    std::string packTetrominoJSON(TetrominoState tetrominoState);
}

#endif /* defined(__Tetrominos__JSONPacker__) */
