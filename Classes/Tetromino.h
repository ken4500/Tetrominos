//
//  Tetromino.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef __Tetrominos__Tetromino__
#define __Tetrominos__Tetromino__

#include "cocos2d.h"
#include "Constants.h"
#include "Coordinate.h"

class Tetromino : public cocos2d::Node
{
public:
    static Tetromino* createWithType(TetrominoType type);
    void rotate(bool right);

    int getHighestYcoodinate();
    int getWidhtInBlocks();
    int getMinimumXCoordinate();
    std::vector<int> getSkirt();
    std::vector<cocos2d::Sprite*> getBlocks();
    std::vector<Coordinate> getCurrentRotation();
    TetrominoType getTetrominoType();
    cocos2d::Color3B getTetrominoColor();

private:
    int rotationIndex;
    TetrominoType type;
    std::vector<std::vector<Coordinate>> rotations;
    cocos2d::Color3B color;
    std::vector<cocos2d::Sprite*> blocks;

    bool initWithType(TetrominoType type);
    void onEnter() override;
    void onExit() override;

};

#endif /* defined(__Tetrominos__Tetromino__) */
