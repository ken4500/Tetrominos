//
//  NextTetromino.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/10.
//
//

#include "NextTetromino.h"
using namespace cocos2d;

bool NextTetromino::init()
{
    if (!LayerColor::initWithColor(Color4B::GRAY, 100, 100)) {
        return false;
    }
    
    this->displayTetromino = nullptr;
    auto label3 = ui::Text::create("NEXT", FONT_NAME, 24);
    label3->setColor(Color3B::BLACK);
    label3->setAnchorPoint(Vec2(0.5f, 0.0f));
    label3->setPosition(Vec2(this->getContentSize().width /2, this->getContentSize().height));
    this->addChild(label3);

    return true;
}

void NextTetromino::setTetromino(TetrominoType type)
{
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
}

TetrominoType NextTetromino::getTetromino()
{
    return this->displayTetromino->getTetrominoType();
}
