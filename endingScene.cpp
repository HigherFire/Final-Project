#include "endingScene.h"
USING_NS_CC;

Scene* endingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = endingScene::create();
    scene->addChild(layer);
    return scene;
}

bool endingScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto background = Sprite::create("bg_ending.jpg");
    background->setScale(0.5);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);
    
    //title
    auto title = Label::create("GAME  OVER", "Arial", 70);
    if (passTheGame == true) {
        title->setString("WELL  DONE!");
    }
    title->setPosition(Vec2(visibleSize.width*0.5+origin.x, visibleSize.height*0.7+origin.y));
    title->setColor(Color3B::WHITE);
    this->addChild(title);
    
    /////////score
    /*
    auto score = Label::create("", "Arial", 60);
    score->setString(StringUtils::format("Score: %d",i_score));
    score->setPosition(Vec2(visibleSize.width*0.5+origin.x, visibleSize.height*0.5+origin.y));
    score->setColor(Color3B::WHITE);
    this->addChild(score);
    *////////////////
    
    //buttons
    auto restartButton = MenuItemLabel::create(Label::createWithSystemFont("重新开始", "Arial", 40), CC_CALLBACK_1(endingScene::endingCallBack, this));
    restartButton->setPosition(Vec2(visibleSize.width*0.35+origin.x, visibleSize.height*0.3+origin.y));
    restartButton->setColor(Color3B::WHITE);
    restartButton->setTag(1);
    
    auto exitButton = MenuItemLabel::create(Label::createWithSystemFont("结束游戏", "Arial", 40), CC_CALLBACK_1(endingScene::endingCallBack, this));
    exitButton->setPosition(Vec2(visibleSize.width*0.65+origin.x, visibleSize.height*0.3+origin.y));
    exitButton->setColor(Color3B::WHITE);
    exitButton->setTag(2);
    
    //menu
    auto endingMenu = Menu::create(restartButton, exitButton, NULL);
    endingMenu->setPosition(Size::ZERO);
    this->addChild(endingMenu, 1);
    
    return true;
}

void endingScene::endingCallBack(Ref* pSender){
    switch (((MenuItemLabel *)pSender)->getTag()) {
        case 1:
            //tsm->toGameScene();
            //记录分数，录入排行榜
            break;
        case 2:
            Director::getInstance()->end();
            exit(0);
            break;
        default:
            break;
    }
}
