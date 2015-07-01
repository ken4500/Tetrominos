//
//  Lobby.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Lobby.h"
#include "SceneManager.h"

using namespace cocos2d;

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);
    
    return true;
}


void Lobby::onEnter()
{
    Node::onEnter();
    this->setupUI();
}

void Lobby::setupUI()
{

    auto displaySize = Director::getInstance()->getVisibleSize();

    auto logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.75f));
    this->addChild(logo);
    
    auto singleButton = ui::Button::create();
    singleButton->loadTextures("singlePlayerButton.png", "singlePlayerButtonPressed.png");
    singleButton->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.4f));
    singleButton->addTouchEventListener(CC_CALLBACK_2(Lobby::singlePlayerButtonPressed, this));
    this->addChild(singleButton);
    
    auto multiButton = ui::Button::create();
    multiButton->loadTextures("multiplayerButton.png", "multiplayerButtonPressed.png");
    multiButton->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.25f));
    multiButton->addTouchEventListener(CC_CALLBACK_2(Lobby::multiPlayerButtonPressed, this));
    this->addChild(multiButton);
}

void Lobby::singlePlayerButtonPressed(cocos2d::Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->enterGameScene(false);
    }
}

void Lobby::multiPlayerButtonPressed(cocos2d::Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->enterGameScene(true);
    }
}
