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

#pragma mark - View lifecycle

Tetromino* Tetromino::createWithType(TetrominoType type)
{
    Tetromino* tetromino = new(std::nothrow) Tetromino();
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
    this->rotationIndex = 0;

    this->blocks = std::vector<Sprite*>();
    this->blocks.reserve(4);
    Sprite* dummyBlack = Sprite::create("block.png");
    Size dummySize = dummyBlack->getContentSize();
    this->setContentSize(Size(dummySize.width * float(GRID_SIZE), dummySize.height * float(GRID_SIZE)));

    auto coordinates = rotations[this->rotationIndex];
    for (Coordinate coordinate : coordinates) {
        auto block = Sprite::create("block.png");
        block->setColor(this->color);
        block->setAnchorPoint(Vec2(0, 0));
        block->setPosition(Vec2(dummySize.width * coordinate.x, dummySize.height * coordinate.y));
        this->addChild(block);
        this->blocks.push_back(block);
    }
    this->rotationIndex = 0;
    
    return true;
}


void Tetromino::onEnter()
{
    Node::onEnter();
}

void Tetromino::onExit()
{
    Node::onExit();
}

#pragma mark - public method

void Tetromino::rotate(bool right)
{
    this->rotationIndex += (right) ? 1 : -1;
    if (this->rotationIndex >= (int) this->rotations.size()) {
        this->rotationIndex = 0;
    } else if (rotationIndex < 0) {
        this->rotationIndex = (int) this->rotations.size() - 1;
    }
    
    auto coordinates = this->rotations[this->rotationIndex];
    for (int i = 0; i < coordinates.size(); i++) {
        auto block = this->blocks[i];
        auto coordinate = coordinates[i];
        auto blockSize = block->getContentSize();
        block->setPosition(Vec2(blockSize.width * coordinate.x, blockSize.height * coordinate.y));
    }
}

int Tetromino::getHighestYcoodinate()
{
    int highestY = 0;
    auto coordinates = this->rotations[this->rotationIndex];
    for (auto p : coordinates) {
        highestY = MAX(p.y, highestY);
    }
    return highestY;
}

int Tetromino::getWidhtInBlocks()
{
    int minX = GRID_SIZE;
    int maxX = 0;
    auto coordinates = this->rotations[this->rotationIndex];
    for (auto p : coordinates) {
        maxX = MAX(p.x, maxX);
        minX = MIN(p.x, minX);
    }
    return maxX - minX + 1;
}

int Tetromino::getMinimumXCoordinate()
{
    int minX = GRID_SIZE;
    auto coordinates = this->rotations[this->rotationIndex];
    for (auto p : coordinates) {
        minX = MIN(p.x, minX);
    }
    return minX;
}

std::vector<int> Tetromino::getSkirt()
{
    std::vector<int> rtn;
    auto coordinates = this->rotations[this->rotationIndex];
    for (int x = 0; x < GRID_SIZE; x++) {
        int min = GRID_SIZE;
        for (auto p : coordinates) {
            if (p.x == x && p.y < min) {
                min = p.y;
            }
        }
        if (min != GRID_SIZE) {
            rtn.push_back(min);
        }
    }
    return rtn;
}

std::vector<cocos2d::Sprite*> Tetromino::getBlocks()
{
    return this->blocks;
}

std::vector<Coordinate> Tetromino::getCurrentRotation()
{
    return this->rotations[this->rotationIndex];
}

TetrominoType Tetromino::getTetrominoType()
{
    return this->type;
}

Color3B Tetromino::getTetrominoColor()
{
    return this->color;
}
