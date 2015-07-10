//
//  Grid.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"
#include "UIConstants.h"

using namespace cocos2d;

#pragma mark - lifecycle

bool Grid::init() {
    if (!Sprite::initWithFile("grid.png")) {
        return false;
    }
    
    this->activeTetromino = nullptr;
    this->activeTetrominoCoordinate = Coordinate();
    this->score = 0;
    this->totalLinesCleared = 0;


    for (int y = 0; y < GRID_HEIGHT; y++) {
        std::vector<Sprite*> row(GRID_WIDTH, nullptr);
        this->blocksLanded.push_back(row);
    }

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
        if (this->checkIfTetrominoCollide(this->activeTetromino, this->activeTetrominoCoordinate)) {
            this->activeTetromino->rotate(false);
        } else if (this->ghostTetromino) {
            this->ghostTetromino->rotate(true);
            this->updateGhostTetrominoPosition();
        }
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
    
    // add ghost
    this->ghostTetromino = Tetromino::createWithType(tetromino->getTetrominoType());
    this->ghostTetromino->setCascadeOpacityEnabled(true);
    this->ghostTetromino->setOpacity(GHOST_TETROMINO_OPACITY);
    this->updateGhostTetrominoPosition();

    this->addChild(this->ghostTetromino);
}

void Grid::step()
{
    if (activeTetromino) {
        Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
        Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y - 1);
        
        if (checkIfTetrominoCollide(activeTetromino, nextCoordinate)) {
            this->deactivateTetromino(activeTetromino, activeCoordinate);
        } else {
            this->setActiveTetrominoCoordinate(nextCoordinate);
        }
    }
}

Tetromino* Grid::getActiveTetromino()
{
    return this->activeTetromino;
}

void Grid::dropActiveTetromino()
{
    if (this->activeTetromino) {
        Coordinate landingCoordinate = this->getTetrominoLandingCoordinate();
        this->setActiveTetrominoCoordinate(landingCoordinate);
    }
}

bool Grid::checkIfTopReached()
{
    for (Sprite* block : blocksLanded.back()) {
        if (block)
        {
            return true;
        }
    }
    return false;
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
    this->updateGhostTetrominoPosition();
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

int Grid::getTotalLinesCleared()
{
    return this->totalLinesCleared;
}

int Grid::getScore()
{
    return this->score;
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
        
        if (this->blocksLanded[gridCoordinate.y][gridCoordinate.x]) {
            return true;
        }
    }
    return false;
}

void Grid::deactivateTetromino(Tetromino* tetromino, Coordinate tetrominoCoordinate)
{
    this->placeTetrominoOnBoard(tetromino, tetrominoCoordinate);
    this->activeTetromino->removeFromParent();
    this->activeTetromino = nullptr;
    this->ghostTetromino->removeFromParent();
    this->ghostTetromino = nullptr;
    this->clearLines();
}

void Grid::placeTetrominoOnBoard(Tetromino* tetromino, Coordinate tetrominoCoordinate)
{
    std::vector<Sprite*> blocks = tetromino->getBlocks();
    std::vector<Coordinate> coordinates = tetromino->getCurrentRotation();
    
    for (int index = 0; index < BLOCKS_IN_TETROMINO; index++) {
        Coordinate globalCoordinate = Coordinate::add(coordinates[index], tetrominoCoordinate);
        
        Sprite* block = blocks[index];
        Vec2 blockPosition = block->getPosition() + tetromino->getPosition();
        block->setPosition(blockPosition);
        
        block->retain();
        block->removeFromParent();
        this->addChild(block);
        block->release();
        if (globalCoordinate.y < GRID_HEIGHT) {
            this->blocksLanded[globalCoordinate.y][globalCoordinate.x] = block;
        }
    }
}

Coordinate Grid::getTetrominoLandingCoordinate()
{
    Coordinate returnCoordinate = this->activeTetrominoCoordinate;
    while (true) {
        returnCoordinate.y--;
        if (checkIfTetrominoCollide(this->activeTetromino, returnCoordinate)) {
            returnCoordinate.y++;
            break;
        }
    }
    return returnCoordinate;
}

void Grid::clearLines()
{
    int lineCleared = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        auto line = blocksLanded[y];
        bool clearLine = true;
        for (Sprite* s : line) {
            if (!s) {
                clearLine = false;
            }
        }
        if (clearLine) {
            this->removeLine(y);
            lineCleared++;
            y--;
        }
    }
    this->totalLinesCleared += lineCleared;
    this->updateScore(lineCleared);
}

void Grid::removeLine(int removeY)
{
    for (int x = 0; x < GRID_WIDTH; x++) {
        if (this->blocksLanded[removeY][x]) {
            this->blocksLanded[removeY][x]->removeFromParent();
            this->blocksLanded[removeY][x] = nullptr;
        }
    }
    for (int y = removeY + 1; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            auto moveTetromino = this->blocksLanded[y][x];
            if (moveTetromino) {
                moveTetromino->setPosition(moveTetromino->getPosition() + Vec2(0, -moveTetromino->getContentSize().height));
            }
            this->blocksLanded[y - 1][x] = this->blocksLanded[y][x];
        }
    }
}

void Grid::updateGhostTetrominoPosition()
{
    if (this->ghostTetromino) {
        Coordinate landingCoordinate = this->getTetrominoLandingCoordinate();
        this->ghostTetromino->setPosition(this->convertCoordinateToPosition(landingCoordinate));
    }
}

void Grid::updateScore(int lineCleard)
{
    this->score += lineCleard;
    if (lineCleard == 4) {
        this->score += 1;
    }
}
