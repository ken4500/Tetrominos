//
//  GameScene.h
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#ifndef __Tetrominos__GameScene__
#define __Tetrominos__GameScene__

#include "cocos2d.h"
#include "CocosGUI.h"
#include "TetrominoBag.h"
#include <time.h>
class Grid;
class Tetromino;
class Coordinate;

class GameScene : public cocos2d::Node
{
public:
    CREATE_FUNC(GameScene);

protected:
    Grid* grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;
    bool active;
    cocos2d::ui::Text* scoreLabel;
    int totalScore;
    float stepInterval;

    // Lifecycle
    bool init() override;
    void onEnter() override;
    void setupTouchHandler();

    // Game Logic
    void setGameActive(bool active);
    Tetromino* createRandomTetromino();
    void step(float dt);
    void updateStateFromScore();
    void updateGameSpeed(int score);

    // UI
    void updateScoreLabel(int score);
    void backButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);

    // Utility
    Coordinate convertPositionToCoordinate(cocos2d::Vec2 position);
    
};

#endif /* defined(__Tetrominos__GameScene__) */
