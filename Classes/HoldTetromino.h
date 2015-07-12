//
//  HoldTetromino.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/11.
//
//

#ifndef __Tetrominos__HoldTetromino__
#define __Tetrominos__HoldTetromino__

#include "Cocos2d.h"
#include "Constants.h"
#include "UIConstants.h"
#include "Tetromino.h"
#include "CocosGUI.h"

class HoldTetromino : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(HoldTetromino);
    TetrominoType holdTetromino(TetrominoType type);

private:
    Tetromino* displayTetromino;

    bool init() override;
    
};


#endif /* defined(__Tetrominos__HoldTetromino__) */
