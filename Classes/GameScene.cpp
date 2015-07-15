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
#include "UIConstants.h"
#include "NetworkingWrapper.h"
#include "JSONPacker.h"
#include "NextTetromino.h"
#include "SoundManager.h"

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
    this->totalScore = 0;
    this->stepInterval = INITIAL_STEP_INTERVAL;
    this->timeLeft = TIME_PER_GAME;
    this->gameIsOver = false;

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

    // score label
    auto label = ui::Text::create("SCORE", FONT_NAME, FONT_SIZE);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2(0.5f, 1.0f));
    label->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.85));
    this->addChild(label);
    
    this->scoreLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->scoreLabel->setColor(LABEL_COLOR);
    this->scoreLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->scoreLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.85 - 60));
    this->addChild(this->scoreLabel);

    // time label
    auto label2 = ui::Text::create("TIME", FONT_NAME, FONT_SIZE);
    label2->setColor(Color3B::BLACK);
    label2->setAnchorPoint(Vec2(0.5f, 1.0f));
    label2->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.95));
    this->addChild(label2);
    
    this->timeLeftLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->timeLeftLabel->setColor(LABEL_COLOR);
    this->timeLeftLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->timeLeftLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.95 - 60));
    this->addChild(this->timeLeftLabel);
    
    // grid
    this->grid = Grid::create();
    this->grid->setAnchorPoint(Vec2(0.5f, 0.0f));
    this->grid->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.0f));
    this->addChild(this->grid);

    // next view
    this->nextTetromino = NextTetromino::create();
    this->nextTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));
    this->nextTetromino->setPosition(Vec2(visibleSize.width - 70, this->grid->getContentSize().height));
    auto type1 = this->tetrominoBag->getTetromino();
    auto type2 = this->tetrominoBag->getTetromino();
    auto type3 = this->tetrominoBag->getTetromino();
    this->nextTetromino->initTetromino(type1, type2, type3);
    this->addChild(this->nextTetromino);
    
    // hold view
    this->holdTetromino = HoldTetromino::create();
    this->holdTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));
    this->holdTetromino->setPosition(Vec2(20, this->grid->getContentSize().height - 100));
    this->addChild(this->holdTetromino);
    
    if (this->networkedSession) {
        this->grid->setAnchorPoint(Vec2(0.0f, 0.0f));
        this->grid->setPosition(Vec2(0.0f, 0.0f));
        this->grid->setScale(0.8f);

        this->previewGrid = PreviewGrid::create();
        this->previewGrid->setAnchorPoint(Vec2(1.0f, 0.0f));
        this->previewGrid->setPosition(Vec2(visibleSize.width, 0));
        this->previewGrid->setScale(0.8f);
        this->addChild(this->previewGrid);
        
        // label
        auto name = NetworkingWrapper::getDeviceName();
        auto label3 = ui::Text::create(name, FONT_NAME, 36);
        label3->setColor(Color3B::BLACK);
        label3->setAnchorPoint(Vec2(0.5f, 0.0f));
        label3->setPosition(Vec2(this->grid->getContentSize().width /2 * 0.8f, this->grid->getContentSize().height * 0.8 + 10));
        this->addChild(label3);


        // label
        auto label4 = ui::Text::create("", FONT_NAME, 36);
        label4->setName("enemy_name_label");
        label4->setColor(Color3B::BLACK);
        label4->setAnchorPoint(Vec2(0.5f, 0.0f));
        label4->setPosition(Vec2(visibleSize.width - this->previewGrid->getContentSize().width /2 * 0.8f, this->previewGrid->getContentSize().height * 0.8 + 10));
        this->addChild(label4);
    }
    
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
//                bool movingRight = (differenseCoordinate.x > 0);
//                newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x + ((movingRight) ? 1 : -1), activeTetrominoCoordinate.y);
                newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x + differenseCoordinate.x, activeTetrominoCoordinate.y);
                this->grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
                lastTouchPos = touchPos;
                allowRotate = false;
                SoundManager::getInstance()->playMoveEffect();
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
            Vec2 difference = firstTouchPos - touchEndPos;
            float touchDuration = (float)(clock() - touchStartedTime) / CLOCKS_PER_SEC;
            float velocity = difference.y / touchDuration;
            CCLOG("velocity = %f", velocity);
            if (velocity > DROP_VELOCITY) {
                this->grid->dropActiveTetromino();
                SoundManager::getInstance()->playDropEffect();

                this->step(0);
            } else if (velocity < HOLD_VELOCITY) {
                this->hold();
                SoundManager::getInstance()->playHoldEffect();
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark - public method

void GameScene::setNetworkedSession(bool networkedSession)
{
    this->networkedSession = networkedSession;
}

void GameScene::receivedData(const void* data, unsigned long length)
{
    const char* cstr = reinterpret_cast<const char*>(data);
    std::string json = std::string(cstr, length);

    JSONPacker::GameState state = JSONPacker::unpackGameStateJSON(json);
    
    auto label = this->getChildByName<ui::Text*>("enemy_name_label");
    label->setString(state.name);

    if (state.gameOver) {
        this->gameOver();
    }
    this->previewGrid->setState(state);
}

#pragma mark - protected method

Tetromino* GameScene::createRandomTetromino()
{
    auto type = this->tetrominoBag->getTetromino();
    auto nextType = this->nextTetromino->getTetromino();
    this->nextTetromino->setTetromino(type);
    return Tetromino::createWithType(nextType);
}

void GameScene::setGameActive(bool active)
{
    if (active) {
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), this->stepInterval);
        this->scheduleUpdate();
    } else {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
        this->unscheduleUpdate();
    }
}

void GameScene::step(float dt)
{
    if (this->grid->checkIfTopReached()) {
        this->gameOver();
    }

    Tetromino* activeTetromino = this->grid->getActiveTetromino();
    if (!activeTetromino) {
        Tetromino* newTetromino = this->createRandomTetromino();
        this->grid->spawnTetromino(newTetromino);
    } else {
        this->grid->step();
        this->updateStateFromScore();
    }
    
    if (this->networkedSession) {
        this->sendGameStateOverNetwork();
    }
}

void GameScene::update(float dt)
{
    Node::update(dt);
    this->setTimeLeft(this->timeLeft - dt);
    
    if (this->timeLeft <= 0.0f) {
        this->gameOver();
    }
}

void GameScene::updateStateFromScore()
{
    int newScore = this->grid->getScore();
    if (newScore > totalScore) {
        this->totalScore = newScore;
        this->updateScoreLabel(newScore);
        this->updateGameSpeed(newScore);
    }
    
}

void GameScene::updateGameSpeed(int score)
{
    int stepAcceleration = score / SCORE_TO_ACCELERATE;
    float intervalDeduction = powf(ACCELERATION_FACTOR, stepAcceleration);
//    float intervalDeduction = INITIAL_STEP_INTERVAL * float(stepAcceleration) * ACCELERATION_FACTOR;
    float newInterval = MAX(INITIAL_STEP_INTERVAL * intervalDeduction, SPEED_MAX);
    this->stepInterval = newInterval;
    this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), this->stepInterval);
    
}

void GameScene::gameOver()
{
    this->setGameActive(false);
    this->gameIsOver = true;
    
    if (this->networkedSession) {
        this->sendGameStateOverNetwork();
    }
    
    // save high score
    int highScore = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_KEY_HIGH_SCORE, 0);
    if (highScore < totalScore) {
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_KEY_HIGH_SCORE, totalScore);
        UserDefault::getInstance()->flush();
    }
    
    std::string scoreString = StringUtils::toString(totalScore);
    std::string messageContent = "Your score is " + scoreString + "!";
    
    MessageBox(messageContent.c_str(), "Game Over");
    
    SceneManager::getInstance()->backLobbyScene();
}

void GameScene::setTimeLeft(float time)
{
    this->timeLeft = time;
    this->timeLeftLabel->setString(StringUtils::format("%.1f", time));
}

void GameScene::hold()
{
    auto currentType = this->grid->getActiveTetromino()->getTetrominoType();
    auto popType = this->holdTetromino->holdTetromino(currentType);
    Tetromino* nextTetromino;
    if (popType == TetrominoType::NONE) {
        nextTetromino = this->createRandomTetromino();
    } else {
        nextTetromino = Tetromino::createWithType(popType);
    }
    this->grid->hold(nextTetromino);
}

#pragma - Network

void GameScene::sendGameStateOverNetwork()
{
    JSONPacker::GameState state;
    state.name = NetworkingWrapper::getDeviceName();
    state.score = this->totalScore;
    state.gameOver = this->gameIsOver;
    auto blocksLanded = this->grid->getBlockLanded();
    std::vector<std::vector<Sprite*>> blocks;
    for (int y = 0; y < blocksLanded.size(); y++) {
        std::vector<Color3B> blocks(blocksLanded.size(), Color3B::WHITE);
        state.board.push_back(blocks);
        
        std::vector<Sprite*> column = blocksLanded[y];
        
        for (int x = 0; x < column.size(); x++) {
            Sprite* block = column[x];
            if (block) {
                state.board[y][x] = block->getColor();
            }
        }
    }
    
    Tetromino* activeTetromino = this->grid->getActiveTetromino();
    if (activeTetromino) {
        std::vector<Coordinate> coordinates = activeTetromino->getCurrentRotation();
        Coordinate tetrominoCoordinate = this->grid->getActiveTetrominoCoordinate();
        
        for (Coordinate blockCoordinate : coordinates) {
            Coordinate gridCoordinate = Coordinate::add(blockCoordinate, tetrominoCoordinate);
            if (gridCoordinate.x < GRID_WIDTH && gridCoordinate.y < GRID_HEIGHT) {
                state.board[gridCoordinate.y][gridCoordinate.x] = activeTetromino->getTetrominoColor();
            }
        }
    }
    
    auto data = JSONPacker::packGameState(state);
    SceneManager::getInstance()->sendData(data.c_str(), data.size());
}

#pragma mark - UI method

void GameScene::backButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    SceneManager::getInstance()->backLobbyScene();
}

void GameScene::updateScoreLabel(int score)
{
    std::string scoreString = StringUtils::toString(score);
    this->scoreLabel->setString(scoreString);
}


#pragma mark - Utility 
Coordinate GameScene::convertPositionToCoordinate(cocos2d::Vec2 position)
{
    Size size = this->grid->getBlockSize();
    return Coordinate((int)(position.x / size.width), (int)(position.y / size.height));
}


