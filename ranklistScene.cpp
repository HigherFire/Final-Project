//this is ranklistScene.cpp

#include "ranklistScene.h"

ranklistScene::ranklistScene(){
    
}


ranklistScene::~ranklistScene(){
    
}


bool ranklistScene:: init(){
    
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    //background
    
    auto background = Sprite::create("bg_ending.jpg");
    
    background->setScale(0.5);
    
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    this->addChild(background, 0);
    
    
    //"返回"button
    
    auto backButton = MenuItemImage::create("button_back.png", "button_back_selected.png", CC_CALLBACK_1(ranklistScene::backCallback, this));
    
    backButton->setAnchorPoint(Vec2(1,0));
    
    backButton->setPosition(Vec2::ZERO);
    
    backButton->setScale(0.3);
    
    auto menu = Menu::create(backButton, NULL);
    
    menu->setPosition(Vec2(visibleSize.width + origin.x, origin.y));
    
    this->addChild(menu);
    
    
    //title
    
    auto title = Label::create("排行榜", "Arial", 30);
    
    title->setAnchorPoint(Vec2(0.5, 1));
    
    title->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height*0.95 + origin.y));
    
    title->setColor(Color3B::WHITE);
    
    this->addChild(title);
    
    
    //data
    
    auto no1 = Label::create("No.1    0", "Arial", 25);
    
    no1->setAnchorPoint(Vec2(0, 0.5));
    
    no1->setPosition(Vec2(visibleSize.width*0.35+origin.x, visibleSize.height*0.625+origin.y));
    
    no1->setColor(Color3B::WHITE);
    
    this->addChild(no1);
    
    
    auto no2 = Label::create("No.2    0", "Arial", 25);
    
    no2->setAnchorPoint(Vec2(0, 0.5));
    
    no2->setPosition(Vec2(visibleSize.width*0.35+origin.x, visibleSize.height*0.5+origin.y));
    
    no2->setColor(Color3B::WHITE);
    
    this->addChild(no2);
    
    auto no3 = Label::create("No.3    0", "Arial", 25);
    
    
    no3->setAnchorPoint(Vec2(0, 0.5));
    
    no3->setPosition(Vec2(visibleSize.width*0.35+origin.x, visibleSize.height*0.375+origin.y));
    
    no3->setColor(Color3B::WHITE);
    
    this->addChild(no3);
    
    
    //更新游戏数据
    /*
    no1->setString(StringUtils::format("No.1    %d",///第一名分数/////));
     
    no2->setString(StringUtils::format("No.2    %d",///第二名分数/////));
     
    no3->setString(StringUtils::format("No.3    %d",///第三名分数/////));
    */
    
    
    return true;
}


Scene* ranklistScene:: createScene(){
    
    auto layer = ranklistScene::create();
    
    auto scene = Scene::create();
    
    scene->addChild(layer);
    
    return scene;
    
}


void ranklistScene:: toMainScene(){
    
    auto scene = MenuLayer::createScene();
    
    Director::getInstance()->replaceScene(scene);
    
}


void ranklistScene:: backCallback(Ref* pSender){
    
    toMainScene();
    
}
