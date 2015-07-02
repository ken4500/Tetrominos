//
//  Grid.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"

using namespace cocos2d;

#pragma mark - lifecycle

bool Grid::init() {
    if (!Sprite::initWithFile("grid.png")) {
        return false;
    }
    
    this->activeTetrimino = nullptr;

    return true;
}

void Grid::onEnter()
{
    Sprite::onEnter();

    this->activeTetrimino = Tetromino::createWithType(TetrominoType::T);
    addChild(this->activeTetrimino);
}

#pragma mark - public method

void Grid::rotateActiveTetromino()
{
    if (this->activeTetrimino) {
        this->activeTetrimino->rotate(false);
    }
    
    // TODO: check collision
}
