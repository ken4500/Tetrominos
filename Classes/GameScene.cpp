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

using namespace cocos2d;

#pragma mark - lifecycle

bool GameScene::init()
{
    if (! Node::init()) {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

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
    
    this->setupTouchHandler();
}

void GameScene::setupTouchHandler()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        return true;
    };
    
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        this->grid->rotateActiveTetromino();
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark - public method

#pragma mark - protected method

#pragma mark - UI method

void GameScene::backButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    SceneManager::getInstance()->backLobbyScene();
}



