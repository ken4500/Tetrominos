//
//  NextTetromino.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/10.
//
//

#include "NextTetromino.h"
using namespace cocos2d;

bool NextTetromino::init()
{
    if (!LayerColor::create(Color4B::GRAY, 100, 100)) {
        return false;
    }
    
    this->displayTetromino = nullptr;

    return true;
}

void setTetromino(TetrominoType type)
{
    
}
