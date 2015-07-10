//
//  Grid.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef __Tetrominos__Grid__
#define __Tetrominos__Grid__

#include "cocos2d.h"
#include "Coordinate.h"
class Tetromino;

class Grid : public cocos2d::Sprite {
public:
    CREATE_FUNC(Grid);
    void rotateActiveTetromino();
    void spawnTetromino(Tetromino* tetromino);
    void step();
    
    void setActiveTetrominoCoordinate(Coordinate coordinate);
    Coordinate getActiveTetrominoCoordinate();
    cocos2d::Size getBlockSize();
    Tetromino* getActiveTetromino();
    void dropActiveTetromino();

protected:
    Tetromino* activeTetromino;
    Tetromino* ghostTetromino;
    Coordinate activeTetrominoCoordinate;
    std::vector<std::vector<Sprite*>> blocksLanded;

    bool init() override;
    void onEnter() override;
    cocos2d::Vec2 convertCoordinateToPosition(Coordinate coordinate);
    bool checkIfTetrominoCollide(Tetromino* tetromino, Coordinate tetrominoCoordinate);
    void deactivateTetromino(Tetromino* tetromino, Coordinate tetrominoCoordinate);
    void placeTetrominoOnBoard(Tetromino* tetromino, Coordinate tetrominoCoordinate);
    Coordinate getTetrominoLandingCoordinate();
    void clearLines();
    void removeLine(int y);
    void updateGhostTetrominoPosition();
};

#endif /* defined(__Tetrominos__Grid__) */
