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
#include "UIConstants.h"
#include "Tetromino.h"
#include "CocosGUI.h"

class NextTetromino : public cocos2d::Node
{
public:
    CREATE_FUNC(NextTetromino);
    void initTetromino(TetrominoType type1, TetrominoType type2, TetrominoType type3);
    void setTetromino(TetrominoType type);
    TetrominoType getTetromino();

private:
    std::vector<cocos2d::Layer*> box;

    bool init() override;
    Tetromino* createTetromino(TetrominoType type);
    
};


#endif /* defined(__Tetrominos__NextTetromino__) */
