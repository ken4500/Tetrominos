//
//  SceneManager.cpp
//  Tetrominos
//
//  Created by Ken Watanabe on 2015/07/01.
//
//

#include "SceneManager.h"
#include "GameScene.h"
#include "SoundManager.h"
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
    gameScene = nullptr;
    this->networkingWrapper = std::unique_ptr<NetworkingWrapper>(new NetworkingWrapper());
    this->networkingWrapper->setDelegate(this);
}

SceneManager::~SceneManager()
{
}

#pragma mark -
#pragma mark Public Methods

void SceneManager::enterGameScene(bool networked)
{
    auto scene = Scene::create();
    this->gameScene = GameScene::create();
    this->gameScene->setNetworkedSession(networked);
    
    scene->addChild(this->gameScene);
    Director::getInstance()->pushScene(scene);
    if (networked) {
        SoundManager::getInstance()->playGameBgm();
    }
}

void SceneManager::backLobbyScene()
{
    if (gameScene) {
        Director::getInstance()->popScene();
        gameScene = nullptr;
        networkingWrapper->disconnect();
        SoundManager::getInstance()->stopBgm();
    }
}

void SceneManager::showPeerList()
{
    networkingWrapper->showPeerList();
    
}

void SceneManager::receiveMultiplayerInvitations()
{
    networkingWrapper->startAdvertisingAvailability();
}

void SceneManager::sendData(const void* data, unsigned long length)
{
    networkingWrapper->sendData(data, length);
}

#pragma - Network delegate

void SceneManager::receivedData(const void* data, unsigned long length)
{
    if (gameScene) {
        gameScene->receivedData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state) {
        case ConnectionState::CONNECTING:
            CCLOG("Connecting...");
            break;
        case ConnectionState::CONNECTED:
            CCLOG("Connected");
            if (!gameScene) {
                this->networkingWrapper->stopAdvertisingAvailability();
                this->enterGameScene(true);
            }
            break;
        case ConnectionState::NOT_CONNECTED:
            CCLOG("Not connected");
            break;
        default:
            break;
    }
}
