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
    if (!Node::init()) {
        return false;
    }
    
    auto label3 = ui::Text::create("NEXT", FONT_NAME, 24);
    label3->setColor(Color3B::BLACK);
    label3->setAnchorPoint(Vec2(0.5f, 0.0f));
    label3->setPosition(Vec2(0, 0));
    this->addChild(label3);
    
    
    auto box1 = LayerColor::create(Color4B(150, 150, 150, 255), 100, 100);
    auto box2 = LayerColor::create(Color4B(190, 190, 190, 255), 100, 100);
    auto box3 = LayerColor::create(Color4B(230, 230, 230, 255), 100, 100);
    box1->setPosition(Vec2(-50, -100));
    box2->setPosition(Vec2(-50, -210));
    box3->setPosition(Vec2(-50, -320));
    addChild(box1);
    addChild(box2);
    addChild(box3);
    box.push_back(box1);
    box.push_back(box2);
    box.push_back(box3);

    return true;
}

void NextTetromino::initTetromino(TetrominoType type1, TetrominoType type2, TetrominoType type3)
{
    box[0]->addChild(createTetromino(type1));
    box[1]->addChild(createTetromino(type2));
    box[2]->addChild(createTetromino(type3));
}

void NextTetromino::setTetromino(TetrominoType type)
{
    box[0]->getChildByName<Tetromino*>("tetromino")->removeFromParent();
    for (int i = (int)box.size() - 1; i > 0; i--) {
        auto tetromino = box[i]->getChildByName<Tetromino*>("tetromino");
        tetromino->retain();
        tetromino->removeFromParent();
        box[i - 1]->addChild(tetromino);
        tetromino->release();
    }
    box[box.size() - 1]->addChild(createTetromino(type));
}

TetrominoType NextTetromino::getTetromino()
{
    auto tetromino = box[0]->getChildByName<Tetromino*>("tetromino");
    return tetromino->getTetrominoType();
}

Tetromino* NextTetromino::createTetromino(TetrominoType type)
{
    auto next = Tetromino::createWithType(type);
    next->setName("tetromino");
    next->setScale(0.5f);
    next->setPosition(Vec2(10, 10));
    return next;
}
