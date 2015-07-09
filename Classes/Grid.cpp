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
    
    this->activeTetromino = nullptr;
    this->activeTetrominoCoordinate = Coordinate();

    return true;
}

void Grid::onEnter()
{
    Sprite::onEnter();

}

#pragma mark - public method

void Grid::rotateActiveTetromino()
{
    if (this->activeTetromino) {
        this->activeTetromino->rotate(true);
    }
    
    if (this->checkIfTetrominoCollide(this->activeTetromino, this->activeTetrominoCoordinate)) {
        this->activeTetromino->rotate(false);
    }
}

void Grid::spawnTetromino(Tetromino* tetromino)
{
    this->activeTetromino = tetromino;
    this->addChild(this->activeTetromino);
    
    // TODO: Place tetromino in correct position in grid
    this->activeTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));
    
    int hightestY = activeTetromino->getHighestYcoodinate();
    int width = activeTetromino->getWidhtInBlocks();
    
    this->setActiveTetrominoCoordinate(Coordinate(GRID_WIDTH / 2 - width / 2 - 1, GRID_HEIGHT - hightestY - 1));
}

void Grid::step()
{
    Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
    Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y - 1);
    this->setActiveTetrominoCoordinate(nextCoordinate);
}

Tetromino* Grid::getActiveTetromino()
{
    return this->activeTetromino;
}

#pragma mark - Setter/ Getters

void Grid::setActiveTetrominoCoordinate(Coordinate coordinate)
{
    if (this->activeTetromino) {
 
        if (!this->checkIfTetrominoCollide(activeTetromino, coordinate)) {
            this->activeTetrominoCoordinate = coordinate;
            Vec2 position = this->convertCoordinateToPosition(coordinate);
            this->activeTetromino->setPosition(position);
        }
    }
}

Coordinate Grid::getActiveTetrominoCoordinate()
{
    return this->activeTetrominoCoordinate;
}

Size Grid::getBlockSize()
{
    Size contentSize = this->getContentSize();
    Size blockSize(contentSize.width / float(GRID_WIDTH), contentSize.height / float(GRID_HEIGHT));
    return blockSize;
}

#pragma mark - protected method

Vec2 Grid::convertCoordinateToPosition(Coordinate coordinate)
{
    Size size = this->getContentSize();
    float width = size.width / float(GRID_WIDTH);
    float height = size.height / float(GRID_HEIGHT);
    
    return Vec2(coordinate.x * width, coordinate.y * height);
}

bool Grid::checkIfTetrominoCollide(Tetromino* tetromino, Coordinate tetrominoCoordinate)
{
    int skirtStart = tetromino->getMinimumXCoordinate();
    std::vector<int> skirt = tetromino->getSkirt();
    
    for (int index = 0; index < skirt.size(); index++) {
        Coordinate localCoordinate = Coordinate(index + skirtStart, skirt[index]);
        Coordinate gridCoordinate = Coordinate::add(tetrominoCoordinate, localCoordinate);
        if (gridCoordinate.x < 0 || gridCoordinate.y < 0 || gridCoordinate.x >= GRID_WIDTH || gridCoordinate.y >= GRID_HEIGHT) {
            return true;
        }
    }
    return false;
}
