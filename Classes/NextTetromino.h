//
//  NextTetromino.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/10.
//
//

#ifndef __Tetrominos__NextTetromino__
#define __Tetrominos__NextTetromino__

#include "Cocos2d.h"
#include "Constants.h"
#include "Tetromino.h"

class NextTetromino : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(NextTetromino);
    void setTetromino(TetrominoType type);
    TetrominoType getTetromino();

private:
    Tetromino* displayTetromino;

    bool init() override;
    
};


#endif /* defined(__Tetrominos__NextTetromino__) */
