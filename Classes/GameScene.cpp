//
//  GameScene.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "GameScene.h"
#include "SceneManager.h"
#include "Grid.h"
#include "Tetromino.h"

using namespace cocos2d;

#pragma mark - lifecycle

bool GameScene::init()
{
    if (! Node::init()) {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);
    this->tetrominoBag = std::unique_ptr<TetrominoBag>(new TetrominoBag());
    this->active = false;

    return true;
}

void GameScene::onEnter()
{
    Node::onEnter();

    // back button
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto backButton = ui::Button::create();
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    backButton->setPosition(Vec2(10, visibleSize.height - 10));
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::backButtonPressed, this));
    this->addChild(backButton);
    
    // grid
    this->grid = Grid::create();
    this->grid->setAnchorPoint(Vec2(0.5f, 0.0f));
    this->grid->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.0f));
    this->addChild(this->grid);
    
    // make random tetromino
    Tetromino* randomTest = this->createRandomTetromino();
    this->grid->spawnTetromino(randomTest);
    
    this->setupTouchHandler();
    
    this->setGameActive(true);
}

void GameScene::setupTouchHandler()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    static Vec2 firstTouchPos;
    static Vec2 lastTouchPos;
    static bool allowRotate;
    static std::clock_t touchStartedTime;

    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        firstTouchPos = this->convertTouchToNodeSpace(touch);
        lastTouchPos = this->convertTouchToNodeSpace(touch);
        allowRotate = true;
        touchStartedTime = clock();
        return true;
    };
    
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        Vec2 touchPos = this->convertTouchToNodeSpace(touch);
        Vec2 difference = touchPos - lastTouchPos;
        Tetromino* activeTetromino = this->grid->getActiveTetromino();
        Coordinate newTetrominoCoordinate;
        
        if (activeTetromino) {
            Coordinate differenseCoordinate = this->convertPositionToCoordinate(difference);
            Coordinate activeTetrominoCoordinate = grid->getActiveTetrominoCoordinate();
            
            if (differenseCoordinate.y <= -1) {
                newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x, activeTetrominoCoordinate.y - 1);
                grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
                lastTouchPos = touchPos;
            } else if (std::abs(differenseCoordinate.x) >= 1) {
                bool movingRight = (differenseCoordinate.x > 0);
                newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x + ((movingRight) ? 1 : -1), activeTetrominoCoordinate.y);
                this->grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
                lastTouchPos = touchPos;
                allowRotate = false;
            }
        }
    };
    
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        Vec2 touchEndPos = this->convertTouchToNodeSpace(touch);
        float distance = touchEndPos.distance(firstTouchPos);
        if (distance < 40.0f && allowRotate) {
            this->grid->rotateActiveTetromino();
        } else {
            Vec2 difference = touchEndPos - firstTouchPos;
            float touchDuration = (float)(clock() - touchStartedTime) / CLOCKS_PER_SEC;
            float velocity = fabsf(difference.y / touchDuration);
            CCLOG("velocity = %f", velocity);
            if (velocity > DROP_VELOCITY) {
                
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark - public method

#pragma mark - protected method

Tetromino* GameScene::createRandomTetromino()
{
    auto type = this->tetrominoBag->getTetromino();
    return Tetromino::createWithType(type);
}

void GameScene::setGameActive(bool active)
{
    if (active) {
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), INITIAL_STEP_INTERVAL);
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    
    }
}

void GameScene::step(float dt)
{
    Tetromino* activeTetromino = this->grid->getActiveTetromino();
    if (!activeTetromino) {
        Tetromino* newTetromino = this->createRandomTetromino();
        this->grid->spawnTetromino(newTetromino);
    } else {
        this->grid->step();
    }
}

#pragma mark - UI method

void GameScene::backButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    SceneManager::getInstance()->backLobbyScene();
}

#pragma mark - Utility 
Coordinate GameScene::convertPositionToCoordinate(cocos2d::Vec2 position)
{
    Size size = this->grid->getBlockSize();
    return Coordinate((int)(position.x / size.width), (int)(position.y / size.height));
}


