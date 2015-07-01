//
//  SceneManager.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "SceneManager.h"
#include "GameScene.h"
using namespace cocos2d;

#pragma mark -
#pragma mark Lifecycle

static SceneManager* _instance = nullptr;
SceneManager* SceneManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new (std::nothrow)SceneManager();
    }
    return _instance;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

#pragma mark -
#pragma mark Public Methods

void SceneManager::enterGameScene(bool networked)
{
    auto scene = Scene::create();
    auto gameScene = GameScene::create();
    scene->addChild(gameScene);
    Director::getInstance()->pushScene(scene);
}

void SceneManager::backLobbyScene()
{
    Director::getInstance()->popScene();
}