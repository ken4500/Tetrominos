//
//  HoldTetromino.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/11.
//
//

#include "HoldTetromino.h"

using namespace cocos2d;

bool HoldTetromino::init()
{
    if (!LayerColor::initWithColor(Color4B::GRAY, 100, 100)) {
        return false;
    }
    
    this->displayTetromino = nullptr;
    auto label3 = ui::Text::create("HOLD", FONT_NAME, 24);
    label3->setColor(Color3B::BLACK);
    label3->setAnchorPoint(Vec2(0.5f, 0.0f));
    label3->setPosition(Vec2(this->getContentSize().width /2, this->getContentSize().height));
    this->addChild(label3);

    return true;
}

TetrominoType HoldTetromino::holdTetromino(TetrominoType type)
{
    TetrominoType rtnValue = TetrominoType::NONE;
    if (this->displayTetromino) {
        rtnValue = this->displayTetromino->getTetrominoType();
    }
    auto pre = this->getChildByName("tetromino");
    if (pre) {
        pre->removeFromParent();
    }
    auto next = Tetromino::createWithType(type);
    this->displayTetromino = next;
    next->setName("tetromino");
    next->setScale(0.5f);
    next->setPosition(Vec2(10, 10));
    addChild(next);
    
    return rtnValue;
}

