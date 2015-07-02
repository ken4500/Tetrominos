//
//  Tetromino.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Tetromino.h"
#include "JSONPacker.h"

#define GRID_SIZE 4

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
    this->rotations = state.rotations;
    this->color = state.color;

    this->blocks = std::vector<Sprite*>(4);
    Sprite* dummyBlack = Sprite::create("block.png");
    Size dummySize = dummyBlack->getContentSize();
    this->setContentSize(Size(dummySize.width * float(GRID_SIZE), dummySize.height * float(GRID_SIZE)));
    
    auto coordinates = rotations[0];
    
    for (Coordinate coordinate : coordinates) {
        auto block = Sprite::create("block.png");
        block->setColor(this->color);
        block->setAnchorPoint(Vec2(0, 0));
        block->setPosition(Vec2(dummySize.width * coordinate.x, dummySize.height * coordinate.y));
        this->addChild(block);
        this->blocks.push_back(block);
    }
    
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
