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
#include "PreviewGrid.h"
#include "HoldTetromino.h"
#include <time.h>
class Grid;
class Tetromino;
class Coordinate;
class NextTetromino;

class GameScene : public cocos2d::Node
{
public:
    static GameScene* create(GameMode mode);
    
    void setNetworkedSession(bool networkedSession);
    void receivedData(const void* data, unsigned long length);
    void step(float dt);

protected:
    Grid* grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;
    bool active;
    cocos2d::ui::Text* scoreLabel;
    cocos2d::ui::Text* timeLeftLabel;
    int totalScore;
    int leftLine;
    float stepInterval;
    float timeLeft;
    bool networkedSession;
    bool gameIsOver;
    PreviewGrid* previewGrid;
    HoldTetromino* holdTetromino;
    NextTetromino* nextTetromino;
    GameMode gameMode;

    // Lifecycle
    bool init(GameMode);
    void onEnter() override;
    void setupTouchHandler();

    // Game Logic
    void setGameActive(bool active);
    Tetromino* createRandomTetromino();
    void update(float dt) override;
    void updateStateFromScore();
    void updateGameSpeed(int score);
    void gameOver();
    void setTimeLeft(float time);
    void hold();

    // Networking
    void sendGameStateOverNetwork();

    // UI
    void updateScoreLabel(int score);
    void backButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);

    // Utility
    Coordinate convertPositionToCoordinate(cocos2d::Vec2 position);
    
};

#endif /* defined(__Tetrominos__GameScene__) */
