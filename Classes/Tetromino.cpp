//
//  Tetromino.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Tetromino.h"
#include "JSONPacker.h"

USING_NS_CC;

Tetromino* Tetromino::createWithType(TetrominoType type)
{
    auto *tetromino = new(std::nothrow) Tetromino();
    if (tetromino && tetromino->initWithType(type))
    {
        tetromino->autorelease();
        return tetromino;
    }
    
    CC_SAFE_DELETE(tetromino);
    return nullptr;
}

bool Tetromino::initWithType(TetrominoType type)
{
    if (!Node::init())
    {
        return false;
    }
    
    this->type = type;
    this->rotations = std::vector<std::vector<Coordinate>>();
    auto jsonStr = FileUtils::getInstance()->getStringFromFile("tetrominos.json");
    auto state = JSONPacker::unpackTetrominoJSON(jsonStr, type);

    
    
    return true;
}

#pragma mark - View lifecycle

void Tetromino::onEnter()
{
    Node::onEnter();
}

void Tetromino::onExit()
{
    Node::onExit();
}
