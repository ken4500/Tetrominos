//
//  Lobby.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "Lobby.h"
#include "SceneManager.h"
#include "Constants.h"
#include "UIConstants.h"

using namespace cocos2d;

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    return true;
}


void Lobby::onEnter()
{
    Node::onEnter();
    
    SceneManager::getInstance()->receiveMultiplayerInvitations();
    
    this->setupUI();
}

void Lobby::setupUI()
{
    this->removeAllChildren();


    auto displaySize = Director::getInstance()->getVisibleSize();

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

    auto logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.75f));
    this->addChild(logo);

    auto line40Button = ui::Button::create();
    line40Button->loadTextures("40lineButton.png", "40lineButtonPressed.png");
    line40Button->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.55f));
    line40Button->addTouchEventListener(CC_CALLBACK_2(Lobby::line40ButtonPressed, this));
    this->addChild(line40Button);
    
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
    
    auto timeHighSocre = UserDefault::getInstance()->getFloatForKey(USERDEFAULT_KEY_TIME_HIGH_SCORE, 9999);
    auto lineHighSocre = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_KEY_HIGH_SCORE, 0);

    auto timeHighScoreLabel = ui::Text::create("40line:", FONT_NAME, FONT_SIZE);
    timeHighScoreLabel->setAnchorPoint(Vec2(1.0f, 0));
    timeHighScoreLabel->setColor(Color3B::BLACK);
    timeHighScoreLabel->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.1f));
    this->addChild(timeHighScoreLabel);

    auto timeHighScoreStr = StringUtils::format("  %.1f", timeHighSocre);
    auto timeHighScoreLabel2 = ui::Text::create(timeHighScoreStr, FONT_NAME, FONT_SIZE);
    timeHighScoreLabel2->setAnchorPoint(Vec2(0.0f, 0));
    timeHighScoreLabel2->setColor(Color3B(162, 63, 63));
    timeHighScoreLabel2->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.1f));
    this->addChild(timeHighScoreLabel2);

    auto highScoreLabel = ui::Text::create("2min:", FONT_NAME, FONT_SIZE);
    highScoreLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
    highScoreLabel->setColor(Color3B::BLACK);
    highScoreLabel->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.1f));
    this->addChild(highScoreLabel);

    auto highScoreLabel2 = ui::Text::create(StringUtils::format("  %d", lineHighSocre), FONT_NAME, FONT_SIZE);
    highScoreLabel2->setAnchorPoint(Vec2(0.0f, 1.0f));
    highScoreLabel2->setColor(Color3B(162, 63, 63));
    highScoreLabel2->setPosition(Vec2(displaySize.width * 0.5f, displaySize.height * 0.1f));
    this->addChild(highScoreLabel2);


}

void Lobby::line40ButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->enterGameScene(false, GameMode::LINE40_MODE);
    }
}

void Lobby::singlePlayerButtonPressed(cocos2d::Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->enterGameScene(false, GameMode::MIN2_MODE);
    }
}

void Lobby::multiPlayerButtonPressed(cocos2d::Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->showPeerList();
    }
}
