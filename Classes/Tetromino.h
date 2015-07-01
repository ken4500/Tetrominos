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

private:
    TetrominoType type;
    std::vector<std::vector<Coordinate>> rotations;

    bool initWithType(TetrominoType type);
    void onEnter() override;
    void onExit() override;

};

#endif /* defined(__Tetrominos__Tetromino__) */
