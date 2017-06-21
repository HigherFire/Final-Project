#include"GameLayer.h"
#include<vector>

USING_NS_CC;

bool GameLevelLayer::init() {
    std::string path = UserDefault::getInstance()->getXMLFilePath();
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("OnLand.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("Jump.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("EatCoin.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("CaiSiGuaiWu.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("DiaoRuXianJingSi.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("DingChuMoGuHuoHua.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("DingYingZhuanKuai.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("EatMushroomOrFlower.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("GameOver.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("QiZiLuoXia.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("SuoXiao.wav");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    screenWidth = visibleSize.width;
    screenHeight = visibleSize.height;
    origin = Director::getInstance()->getVisibleOrigin();
    
    if (UserDefault::getInstance()->getIntegerForKey("game") == 1) {
        map = TMXTiledMap::create("MarioMap1.tmx");
    }
    if (UserDefault::getInstance()->getIntegerForKey("game")==2) {
        map = TMXTiledMap::create("MarioMap5.tmx");
    }
    if (UserDefault::getInstance()->getIntegerForKey("game") == 3) {
        map = TMXTiledMap::create("MarioMap8.tmx");
    }
    map->setAnchorPoint(Vec2::ZERO);
    map->setPosition(Point::ZERO);
    scale = screenHeight / (map->getMapSize().height*map->getTileSize().height);
    map->setScale(scale);
    this->addChild(map, -1);
    //加载地图
    
    
    auto objects = map->getObjectGroup("objects");
    ValueMap start = objects->getObject("startpoint");
    ValueMap value = objects->getObject("finalpoint");
    float startx = value.at("x").asFloat();
    float starty = value.at("y").asFloat();
    final = value.at("x").asFloat();
    Point position;
    if (UserDefault::getInstance()->getFloatForKey("x") == 0) {
        position = Vec2(20, 80);
    }
    else {
        position = Vec2(UserDefault::getInstance()->getFloatForKey("x"), UserDefault::getInstance()->getFloatForKey("y"));
    }
    SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wav", 1);
    _player = Player::create("MarioStand.png");
    _player->isAlive = true;
    _player->isPassed = false;
    _player->setPosition(position);
    setViewpointCenter(_player->getPosition());
    _player->setAnchorPoint(Vec2(0.5f, 0));
    _player->setScale(1);
    _player->setTag(1);
    _player->isOnGround = false;
    auto body = PhysicsBody::createBox(_player->getContentSize());
    body->setRotationEnable(false);
    body->setEnabled(true);
    body->setCategoryBitmask(0x01);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(0);
    PhysicsShape* ps = body->getShape(0);
    ps->setMass(0.2f);
    ps->setFriction(0);
    ps->setDensity(0.2f);
    ps->setRestitution(0);
    _player->setPhysicsBody(body);
    this->addChild(_player, 1);//设置人物
    
    auto objs = objects->getObjects();
    for (int i=0;i<objs.size();i++)
    {
        auto monsterPoint = objs.at(i).asValueMap();
        float monsterX = monsterPoint.at("x").asFloat();
        float monsterY = monsterPoint.at("y").asFloat();
        String type = monsterPoint.at("type").asString();;
        
        if (type._string == "mushroom")
        {
            Monster* _Monster = Monster::create("enemy3.png");
            _Monster->setScale(1);
            _Monster->setTag(2);
            _Monster->setAnchorPoint(Vec2(0.5f, 0));
            auto body_m = PhysicsBody::createBox(_Monster->getContentSize());
            body_m->setRotationEnable(false);
            body_m->setEnabled(true);
            body_m->setCategoryBitmask(0x01);
            body_m->setContactTestBitmask(0x01);
            body_m->setCollisionBitmask(0x01);
            body_m->setGroup(3);
            PhysicsShape* ps_m = body_m->getShape(0);
            ps_m->setMass(0.2f);
            ps_m->setFriction(0);
            ps_m->setDensity(0.2f);
            ps_m->setRestitution(0);
            _Monster->setPhysicsBody(body_m);
            this->addChild(_Monster, 1);
            _Monster->getPhysicsBody()->setVelocity(Vec2(-100, 0));
            _Monster->setPosition(Point(monsterX*scale, monsterY*scale + 30));
            enemyrun(_Monster);
        }
    //怪物生成
    
    if (type._string == "MushroomReward")
        
    {
        
        Unit*_unit = Unit::create("mushroom.png");
        
        _unit->setScale(2);
        
        _unit->setTag(3);
        
        auto body_u = PhysicsBody::createBox(_unit->getContentSize());
        
        body_u->setRotationEnable(false);
        
        body_u->setEnabled(true);
        
        body_u->setCategoryBitmask(0x01);
        
        body_u->setContactTestBitmask(0x01);
        
        body_u->setCollisionBitmask(0x01);
        
        body_u->setGroup(1);
        
        PhysicsShape* ps_u = body_u->getShape(0);
        
        ps_u->setMass(0.2f);
        
        ps_u->setFriction(0);
        
        ps_u->setDensity(0.2f);
        
        
        
        ps_u->setRestitution(1.1);
        
        
        
        _unit->setPhysicsBody(body_u);
        
        this->addChild(_unit, 1);
        
        _unit->setPosition(Point(X*scale, Y*scale));
        
        _unit->setVisible(false);
        
    }
    }
    

    
    Land = map->getLayer("land");
    
    int gid;
    for (int i = 0; i < Land->getLayerSize().width; i++) {
        for (int j = 0; j < Land->getLayerSize().height; j++) {
            gid = Land->getTileGIDAt(Vec2(i, j));
            if (gid) {
                
                //(Land->getTileAt(Vec2(i, 12)))->setAnchorPoint(Vec2(0.5f, 1.0f));
                (Land->getTileAt(Vec2(i, j)))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
                                                                                              map->getTileSize().height)));
                (Land->getTileAt(Vec2(i, j)))->getPhysicsBody()->setCategoryBitmask(0x01);
                (Land->getTileAt(Vec2(i, j)))->getPhysicsBody()->setContactTestBitmask(0x01);
                (Land->getTileAt(Vec2(i, j)))->getPhysicsBody()->setCollisionBitmask(0x01);
                (Land->getTileAt(Vec2(i,j)))->getPhysicsBody()->setGroup(1);
            }
        }
    }
    Land_ = map->getLayer("land_");
    
    for (int i = 0; i < Land_->getLayerSize().width; i++) {
        for (int j = 0; j < Land_->getLayerSize().height; j++) {
            gid = Land_->getTileGIDAt(Vec2(i, j));
            if (gid) {
                
                //(Land->getTileAt(Vec2(i, 12)))->setAnchorPoint(Vec2(0.5f, 1.0f));
                (Land_->getTileAt(Vec2(i, j)))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
                                                                                               map->getTileSize().height)));
                (Land_->getTileAt(Vec2(i, j)))->getPhysicsBody()->setCategoryBitmask(0x01);
                (Land_->getTileAt(Vec2(i, j)))->getPhysicsBody()->setContactTestBitmask(0x01);
                (Land_->getTileAt(Vec2(i, j)))->getPhysicsBody()->setCollisionBitmask(0x01);
                (Land_->getTileAt(Vec2(i, j)))->getPhysicsBody()->setGroup(3);
            }
        }
    }
    
    Block = map->getLayer("block");
    for (int i = 0; i < Block->getLayerSize().width; i++) {
        for (int j = 0; j < Block->getLayerSize().height; j++) {
            gid = Block->getTileGIDAt(Vec2(i, j));
            if (gid) {
                //Block->getTileAt(Vec2(i, j))->setAnchorPoint(Vec2(0.5f, 1.0f));
                Block->getTileAt(Vec2(i, j))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
                                                                                             map->getTileSize().height)));
                Block->getTileAt(Vec2(i, j))->getPhysicsBody()->setCategoryBitmask(0x01);
                Block->getTileAt(Vec2(i, j))->getPhysicsBody()->setCollisionBitmask(0x01);
                Block->getTileAt(Vec2(i, j))->getPhysicsBody()->setContactTestBitmask(0x01);
                Block->getTileAt(Vec2(i, j))->getPhysicsBody()->setGroup(1);
            }
        }
    }
    Block_ = map->getLayer("block_");
    for (int i = 0; i < Block_->getLayerSize().width; i++) {
        for (int j = 0; j < Block_->getLayerSize().height; j++) {
            gid = Block_->getTileGIDAt(Vec2(i, j));
            if (gid) {
                //Block->getTileAt(Vec2(i, j))->setAnchorPoint(Vec2(0.5f, 1.0f));
                Block_->getTileAt(Vec2(i, j))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
                                                                                              map->getTileSize().height)));
                Block_->getTileAt(Vec2(i, j))->getPhysicsBody()->setCategoryBitmask(0x01);
                Block_->getTileAt(Vec2(i, j))->getPhysicsBody()->setCollisionBitmask(0x01);
                Block_->getTileAt(Vec2(i, j))->getPhysicsBody()->setContactTestBitmask(0x01);
                Block_->getTileAt(Vec2(i, j))->getPhysicsBody()->setGroup(3);
            }
        }
    }
    Coin = map->getLayer("coin");
    for (int i = 0; i < Coin->getLayerSize().width; i++) {
        for (int j = 0; j <Coin->getLayerSize().height; j++) {
            gid = Coin->getTileGIDAt(Vec2(i, j));
            if (gid) {
                Coin->getTileAt(Vec2(i, j))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
                                                                                            map->getTileSize().height)));
                Coin->getTileAt(Vec2(i, j))->getPhysicsBody()->setCategoryBitmask(0x01);
                Coin->getTileAt(Vec2(i, j))->getPhysicsBody()->setContactTestBitmask(0x01);
                Coin->getTileAt(Vec2(i, j))->getPhysicsBody()->setCollisionBitmask(0x02);
                Coin->getTileAt(Vec2(i, j))->getPhysicsBody()->setGroup(2);
            }
        }
    }
    Flag = map->getLayer("flagpole");
    
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameLevelLayer::onContactBegin, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(GameLevelLayer::onContactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_2(GameLevelLayer::onContactPostSolve, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameLevelLayer::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameLevelLayer::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameLevelLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //键盘监听
    timerLayer = Layer::create();
    //定时器，游戏时间5分钟//////
    timerLabel = Label::create();
    timerLabel->setTextColor(Color4B::WHITE);
    timerLabel->setSystemFontSize(30);
    timerLabel->setAnchorPoint(Vec2(0, 1));
    timerLabel->setPosition(Vec2(20, visibleSize.height));
    timerLayer->addChild(timerLabel);
    startTime = time(NULL);
    schedule(schedule_selector(GameLevelLayer::timer));
    scheduleOnce(schedule_selector(GameLevelLayer::updateStart), 0);
    //声音按钮
    sound = true;
    if (SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 1) {
        log("isPlayingBGM");
        soundLabel = MenuItemLabel::create(Label::createWithSystemFont("Sound:On", "Arial", 30), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
        soundLabel->setTag(11);
    }
    else {
        soundLabel = MenuItemLabel::create(Label::createWithSystemFont("Sound:Off", "Arial", 30), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
        soundLabel->setTag(10);
    }
    soundLabel->setAnchorPoint(Vec2(0.5, 1));
    soundLabel->setPositionX(visibleSize.width / 2);
    soundLabel->setColor(Color3B::WHITE);
    //返回按钮
    saveLabel = MenuItemLabel::create(Label::createWithSystemFont("Save", "Arial", 30), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
    saveLabel->setTag(20);
    saveLabel->setAnchorPoint(Vec2(1, 1));
    saveLabel->setPositionX(visibleSize.width - 20);
    saveLabel->setColor(Color3B::WHITE);
    //menu
    auto menu = Menu::create(soundLabel, saveLabel, NULL);
    menu->setPosition(origin.x, timerLabel->getPositionY());
    timerLayer->addChild(menu);
    this->addChild(timerLayer, 200);
    return true;
}



void GameLevelLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
    keys[keycode] = true;
    auto rightarrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    auto leftarrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    auto uparrow = EventKeyboard::KeyCode::KEY_UP_ARROW;
    auto downarrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    if (keycode == rightarrow&&_player->isAlive&&_player->isPassed==false) {
        smallWalkRight();
    }
    if (keycode == leftarrow&&_player->isAlive&&_player->isPassed == false) {
        smallWalkLeft();
    }
}



void GameLevelLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event) {
    keys[keycode] = false;
    auto rightarrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    auto leftarrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    auto uparrow = EventKeyboard::KeyCode::KEY_UP_ARROW;
    auto downarrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    if (keycode == rightarrow) {
        _player->stopActionByTag(11);
    }
    if (keycode == leftarrow) {
        _player->stopActionByTag(12);
    }
}



void GameLevelLayer::update(float delta) {
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->step(1.0f / 180.0f);
    int x = _player->getPositionX();
    if (x < screenWidth / 2) {
        x = screenWidth / 2;
    }
    if (x > map->getMapSize().width*map->getTileSize().width*scale - screenWidth / 2) {
        x = map->getMapSize().width*map->getTileSize().width*scale - screenWidth / 2;
    }
    if (_player->isAlive) {
        timerLayer->setPositionX(x - screenWidth / 2);
    }
    auto rightarrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    auto leftarrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    auto uparrow = EventKeyboard::KeyCode::KEY_UP_ARROW;
    auto downarrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    if (isKeyPressed(rightarrow) && _player->isAlive == true && _player->isPassed == false) {
        keyPressedDuration(rightarrow, delta);
    }
    if (isKeyPressed(leftarrow) && _player->isAlive == true && _player->isPassed == false) {
        keyPressedDuration(leftarrow, delta);
    }
    if (!isKeyPressed(rightarrow) && _player->getPhysicsBody()->getVelocity().x>0&&_player->isPassed==false) {
        _player->getPhysicsBody()->setVelocity(Vect(0, _player->getPhysicsBody()->getVelocity().y));
    }
    if (!isKeyPressed(leftarrow) && _player->getPhysicsBody()->getVelocity().x<0&&_player->isPassed==false) {
        _player->getPhysicsBody()->setVelocity(Vect(0, _player->getPhysicsBody()->getVelocity().y));
    }
    if (isKeyPressed(uparrow) && _player->isAlive == true && _player->isPassed == false) {
        keyPressedDuration(uparrow, delta);
    }
    
    if (_player->getPosition().y <= 0 && _player->isAlive == true) {
        
        _player->isAlive = false;
        _player->initWithFile("die.png");
        playerdie();
    }
    int gid;
    Point position = _player->getPosition();
    if (position.y < screenHeight&&position.y>0 && position.x > 0 && position.x < map->getMapSize().width*map->getTileSize().width*scale) {
        position = tileCoordForPosition(position);
        gid = Flag->getTileGIDAt(position);
        if (gid&&_player->isPassed == false) {
            _player->isPassed = true;
            toFlag();
            scheduleOnce(schedule_selector(GameLevelLayer::toEndingScene), 6);
        }
    }
}

bool GameLevelLayer::isKeyPressed(EventKeyboard::KeyCode keycode) {
    
    if (keys[keycode]) {
        return true;
    }
    else {
        return false;
    }
}


void GameLevelLayer::keyPressedDuration(EventKeyboard::KeyCode keycode, float delta) {
    switch (keycode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            if (_player->getPhysicsBody()->getVelocity().x > -1500) {
                _player->getPhysicsBody()->applyImpulse(Vect(-1500, 0));
            }
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (_player->getPhysicsBody()->getVelocity().x < 1500) {
                _player->getPhysicsBody()->applyImpulse(Vect(1500, 0));
            }
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            if (_player->isOnGround) {
                if (sound) {
                    SimpleAudioEngine::getInstance()->playEffect("Jump.wav");
                }
                if (_player->getPhysicsBody()->getVelocity().y < 2000) {
                    _player->getPhysicsBody()->applyImpulse(Vect(0, 20000));
                    log("jump");
                }
                
            }
            break;
    }
    setViewpointCenter(_player->getPosition());
}







void GameLevelLayer::setViewpointCenter(Vec2 position) {
    int x = MAX(position.x, screenWidth / 2);
    x = MIN(x, (map->getMapSize().width*map->getTileSize().width*scale - screenWidth / 2));
    Point actualPosition = Vec2(x, 0);
    Point center = Vec2(screenWidth / 2, 0);
    Point viewPoint = center - actualPosition;
    this->setPosition(viewPoint);
}



bool GameLevelLayer::onContactBegin(PhysicsContact& contact) {
    Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
    if (spriteA&&spriteB) {
        if (spriteA->getTag() == 1 && spriteB->getPhysicsBody()->getGroup() == 1 || spriteA->getPhysicsBody()->getGroup() == 1 && spriteB->getTag() == 1) {
            float half = map->getTileSize().height*scale;
            if (spriteB->getPhysicsBody()->getGroup() == 1) {
                
                if (_player->getPosition().y >= spriteB->getPosition().y+half) {
                    
                    _player->isOnGround = true;
                    log("ready to jump");
                }
                
            }
            
            if (spriteA->getPhysicsBody()->getGroup() == 1) {
                
                if (_player->getPosition().y >= spriteA->getPosition().y+half) {
                    
                    _player->isOnGround = true;
                    log("ready to jump");
                }
            }
            
        }
        if (spriteA->getTag() == 1 && spriteB->getPhysicsBody()->getGroup() == 2 || spriteA->getPhysicsBody()->getGroup() == 2 && spriteB->getTag() == 1) {
            if (spriteB->getPhysicsBody()->getGroup() == 2) {
                spriteB->removeFromParent();
            }
            if (spriteA->getPhysicsBody()->getGroup() == 2) {
                spriteA->removeFromParent();
            }
            if (sound) {
                SimpleAudioEngine::getInstance()->playEffect("EatCoin.wav");
            }
            int score = UserDefault::getInstance()->getIntegerForKey("score");
            
            score = score + 100;
            
            UserDefault::getInstance()->setIntegerForKey("score", score);
            
            UserDefault::getInstance()->flush();
            
            log("%d", score);
        }
        else
        {
            if (spriteA->getTag() == 2 && spriteA->getPhysicsBody()->getGroup() == 3 && spriteB->getPhysicsBody()->getGroup() == 1 && spriteA->getPosition().y == spriteB->getPosition().y)
             {
             if (spriteA->getPhysicsBody()->getVelocity().x<0)
             {
             spriteA->getPhysicsBody()->setVelocity(Vec2(1000, 0));
             }
             else if (spriteA->getPhysicsBody()->getVelocity().x>0)
             {
             spriteA->getPhysicsBody()->setVelocity(Vec2(-1000, 0));
             }
             }
             if (spriteB->getTag() == 2 && spriteB->getPhysicsBody()->getGroup() == 3 && spriteA->getPhysicsBody()->getGroup() == 1 && spriteA->getPosition().y == spriteB->getPosition().y)
             {
             if (spriteB->getPhysicsBody()->getVelocity().x<0)
             {
             spriteB->getPhysicsBody()->setVelocity(Vec2(100, 0));
             }
             else if (spriteB->getPhysicsBody()->getVelocity().x>0)
             {
             spriteB->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             }
             if (spriteB->getTag() == 2 && spriteB->getPhysicsBody()->getGroup() == 3 && spriteA->getPhysicsBody()->getGroup() == 3 && spriteA->getTag() == 2 && spriteA->getPosition().y == spriteB->getPosition().y)
             {
             if (spriteA->getPhysicsBody()->getVelocity().x<0 && spriteB->getPhysicsBody()->getVelocity().x<0)
             {
             if (spriteA->getPhysicsBody()->getPosition().x>spriteB->getPhysicsBody()->getPosition().x)
             {
             spriteB->getPhysicsBody()->setVelocity(Vec2(150, 0));
             spriteA->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             if (spriteA->getPhysicsBody()->getPosition().x<spriteB->getPhysicsBody()->getPosition().x)
             {
             spriteA
             ->getPhysicsBody()->setVelocity(Vec2(150, 0));
             spriteB->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             }
             if (spriteA->getPhysicsBody()->getVelocity().x>0 && spriteB->getPhysicsBody()->getVelocity().x<0)
             {
             spriteA->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             spriteB->getPhysicsBody()->setVelocity(Vec2(100, 0));
             }
             if (spriteA->getPhysicsBody()->getVelocity().x<0 && spriteB->getPhysicsBody()->getVelocity().x>0)
             {
             spriteA->getPhysicsBody()->setVelocity(Vec2(100, 0));
             spriteB->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             if (spriteA->getPhysicsBody()->getVelocity().x>0 && spriteB->getPhysicsBody()->getVelocity().x>0)
             {
             if (spriteA->getPhysicsBody()->getPosition().x>spriteB->getPhysicsBody()->getPosition().x)
             {
             spriteA->getPhysicsBody()->setVelocity(Vec2(150, 0));
             spriteB->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             if (spriteA->getPhysicsBody()->getPosition().x<spriteB->getPhysicsBody()->getPosition().x)
             {
             spriteB->getPhysicsBody()->setVelocity(Vec2(150, 0));
             spriteA->getPhysicsBody()->setVelocity(Vec2(-100, 0));
             }
             }
             }
            if (spriteA->getTag() == 1 && spriteA->getPhysicsBody()->getGroup() != 3 && spriteB->getPhysicsBody()->getGroup() == 3 && spriteA->getPosition().y > spriteB->getPosition().y + spriteB->getContentSize().height)
            {
                
                enemydie(spriteB);
                if (sound) {
                    SimpleAudioEngine::getInstance()->playEffect("Enemydie.wav");
                    int score = UserDefault::getInstance()->getIntegerForKey("score");
                    score = score + 500;
                    UserDefault::getInstance()->setIntegerForKey("score", score);
                    UserDefault::getInstance()->flush();
                }
            }
            if (spriteA->getPhysicsBody()->getGroup() == 3 && spriteB->getPhysicsBody()->getGroup() != 3 && spriteB->getTag() == 1 && spriteA->getPosition().y + spriteA->getContentSize().height<spriteB->getPosition().y)
            {
                enemydie(spriteA);
                if (sound) {
                    SimpleAudioEngine::getInstance()->playEffect("Enemydie.wav");
                    int score = UserDefault::getInstance()->getIntegerForKey("score");
                    score = score + 500;
                    UserDefault::getInstance()->setIntegerForKey("score", score);
                    UserDefault::getInstance()->flush();
                }
                
            }
            if (spriteA->getTag() == 1 && spriteB->getTag() == 2) {
                if (spriteA->getPosition().y < spriteB->getContentSize().height + spriteB->getPosition().y) {
                    playerdie();
                }
                
            }
            
            if (spriteB->getTag() == 1 && spriteA->getTag() == 2) {
                if (spriteB->getPosition().y < spriteA->getContentSize().height + spriteA->getPosition().y) {
                    playerdie();
                }
                
            }
            
        
            
            if (spriteA->getTag() == 1 && spriteB->getTag() == 3 && spriteA->getPosition().y < spriteB->getPosition().y)
                
            {
                
                spriteB->setScale(3);
                
                spriteB->setVisible(true);
                
                spriteB->getPhysicsBody()->setVelocity(Vec2(100, 0));
                
                SimpleAudioEngine::getInstance()->playEffect("DingChuMoGuHuoHua.wav");
                
                
                
            }
            
            if (spriteA->getTag() == 3 && spriteB->getTag() == 1 && spriteA->getPosition().y > spriteB->getPosition().y)
                
            {
                
                spriteA->setScale(3);
                
                spriteA->setVisible(true);
                
                spriteA->getPhysicsBody()->setVelocity(Vec2(100, 0));
                
                SimpleAudioEngine::getInstance()->playEffect("DingChuMoGuHuoHua.wav");
                
            }
        }
    }
    
    return true;
    
}

bool GameLevelLayer::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve) {
    return true;
}



void GameLevelLayer::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve) {
}

void GameLevelLayer::onContactSeparate(PhysicsContact& contact) {
    Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
    if (spriteA&&spriteB) {
        if (spriteA->getTag() == 1 && spriteB->getPhysicsBody()->getGroup() == 1 || spriteA->getPhysicsBody()->getGroup() == 1 && spriteB->getTag() == 1) {
            _player->isOnGround = false;
            
        }
    }
}
void GameLevelLayer::smallWalkRight() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("smallWalkRight.plist");
    auto animation = Animation::create();
    for (int i = 1; i <11; i++) {
        std::string szName = StringUtils::format("smallWalkRight%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animation->setDelayPerUnit(1.0f / 15.0f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "smallWalkRight");
    auto smallWalkRight = AnimationCache::getInstance()->getAnimation("smallWalkRight");
    auto animate = Animate::create(smallWalkRight);
    auto repeat = RepeatForever::create(animate);
    repeat->setTag(11);
    _player->runAction(repeat);
}



void GameLevelLayer::smallWalkLeft() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("smallWalkLeft.plist");
    auto animation = Animation::create();
    for (int i = 1; i <11; i++) {
        std::string szName = StringUtils::format("smallWalkLeft%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animation->setDelayPerUnit(1.0f / 15.0f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "smallWalkLeft");
    auto smallWalkLeft = AnimationCache::getInstance()->getAnimation("smallWalkLeft");
    auto animate = Animate::create(smallWalkLeft);
    auto repeat = RepeatForever::create(animate);
    repeat->setTag(12);
    _player->runAction(repeat);
}

void GameLevelLayer::walkRight() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("walkRight.plist");
    auto animation = Animation::create();
    for (int i = 1; i <11; i++) {
        std::string szName = StringUtils::format("walkRight%d.gif", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animation->setDelayPerUnit(1.0f / 15.0f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "walkRight");
    auto walkRight = AnimationCache::getInstance()->getAnimation("walkRight");
    auto animate = Animate::create(walkRight);
    auto repeat = RepeatForever::create(animate);
    repeat->setTag(13);
    _player->runAction(repeat);
}

void GameLevelLayer::walkLeft() {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("walkLeft.plist");
    auto animation = Animation::create();
    for (int i = 1; i <11; i++) {
        std::string szName = StringUtils::format("walkLeft%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animation->setDelayPerUnit(1.0f / 15.0f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "walkLeft");
    auto walkLeft = AnimationCache::getInstance()->getAnimation("walkLeft");
    auto animate = Animate::create(walkLeft);
    auto repeat = RepeatForever::create(animate);
    repeat->setTag(14);
    _player->runAction(repeat);
}

void GameLevelLayer::enemyrun(Monster*szeName) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemyrun.plist");
    auto animation = Animation::create();
    for (int i = 1; i <4; i++) {
        std::string szName = StringUtils::format("enemy%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
    }
    animation->setDelayPerUnit(1.0f /4.0f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "enemyrun");
    auto enemydie = AnimationCache::getInstance()->getAnimation("enemyrun");
    auto animate = Animate::create(enemydie);
    auto repeat = RepeatForever::create(animate);
    szeName->runAction(repeat);
}


void GameLevelLayer::enemydie(Sprite*Name) {
    Name->stopAllActions();
    scheduleUpdate();
    Name->initWithFile("enemy5.png");
    Name->getPhysicsBody()->setVelocity(Vec2::ZERO);
    Name->getPhysicsBody()->setCategoryBitmask(0x02);
    Name->getPhysicsBody()->setContactTestBitmask(0x02);
    Name->getPhysicsBody()->setCollisionBitmask(0x02);
    Name->getPhysicsBody()->setVelocity(Vec2(0, 150));
}

Vec2 GameLevelLayer::tileCoordForPosition(const Vec2& position) {
    Size mapsize = map->getMapSize();
    Size tilesize = map->getTileSize()*scale;
    int x = position.x / tilesize.width;
    int y = (mapsize.height*tilesize.height - position.y) / tilesize.height;
    return Vec2(x, y);
}



void GameLevelLayer::updatewhenjump() {
    auto rightarrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    auto leftarrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    auto uparrow = EventKeyboard::KeyCode::KEY_UP_ARROW;
    auto downarrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    _player->stopAllActions();
    log("jump");
    if (keys[rightarrow]) {
        smallWalkRight();
    }
    if (keys[leftarrow]) {
        smallWalkLeft();
    }
}



void GameLevelLayer::playerdie() {
    _player->isAlive = false;
    _player->stopAllActions();
    _player->getPhysicsBody()->setVelocity(Vec2::ZERO);
    _player->getPhysicsBody()->setVelocity(Vec2(0, 800));
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    if (sound) {
        SimpleAudioEngine::getInstance()->playEffect("DiaoRuXianJingSi.wav");
    }
    scheduleOnce(schedule_selector(GameLevelLayer::toEndingScene),4.0f);
    
}



void GameLevelLayer::timer(float dt) {
    double leftTime = 300;
    if (UserDefault::getInstance()->getDoubleForKey("leftTime")) {
        leftTime = UserDefault::getInstance()->getDoubleForKey("leftTime");
    }
    double usedTime_s = leftTime - difftime(time(NULL), startTime);
    if (usedTime_s >= 0) {
        double usedTime_min = usedTime_s / 60;
        timerLabel->setString(StringUtils::format("%02d:%02d", (int)usedTime_min, (int)(usedTime_s) % 60));
    }
    else {
        unschedule(schedule_selector(GameLevelLayer::timer));
        //执行游戏结束的函数
        playerdie();
        _player->isAlive = false;
        scheduleOnce(schedule_selector(GameLevelLayer::toEndingScene), 5);
    }
    
    
    
}

void GameLevelLayer::updateStart(float dt) {
    
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setAutoStep(false);
    
    scheduleUpdate();
}



Scene* GameLevelLayer::createScene() {
    
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -1000.0f));
    log("start");
    auto layer = GameLevelLayer::create();
    scene->addChild(layer);
    
    return scene;
    
}



void GameLevelLayer::menuCallBack(Ref* pSender) {
    
    switch (((MenuItemImage *)pSender)->getTag()) {
        case 10:
            sound = true;
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            log("playMusic");
            soundLabel->setString("Sound:On");
            soundLabel->setTag(11);
            break;
        case 11:
            sound = false;
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            log("stop Music");
            soundLabel->setString("Sound:Off");
            soundLabel->setTag(10);
            break;
            
        case 20:
            UserDefault::getInstance()->setFloatForKey("x", _player->getPositionX());
            UserDefault::getInstance()->setFloatForKey("y", _player->getPositionY());
            double previousLeftTime = UserDefault::getInstance()->getDoubleForKey("leftTime");
            UserDefault::getInstance()->setDoubleForKey("leftTime",previousLeftTime-difftime(time(NULL), startTime));
            log("leftTime:%lf", UserDefault::getInstance()->getDoubleForKey("leftTime"));
            UserDefault::getInstance()->flush();
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            SimpleAudioEngine::getInstance()->stopAllEffects();
            toMainScene();
            break;
    }
    
}



void GameLevelLayer::toEndingScene(float dt) {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = Scene::create();
    auto layer = endingScene::create();
    layer->passTheGame = _player->isAlive ;
    layer->addTitle();
    scene->addChild(layer);
    UserDefault::getInstance()->setFloatForKey("x", 20);
    UserDefault::getInstance()->setFloatForKey("y", 80);
    
    if (sound&&!_player->isAlive) {
        SimpleAudioEngine::getInstance()->playEffect("GameOver.wav");
    }
    Director::getInstance()->replaceScene(scene);
}



void GameLevelLayer::toMainScene() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
    auto scene = MenuLayer::createScene();
    Director::getInstance()->replaceScene(scene);
}
void GameLevelLayer::toFinalPoint(float dt) {
    
    if (!_player->isOnGround) {
        log("to final");
    }
    
}
void GameLevelLayer::toNextGame(float dt) {
    log("???");
    Director::getInstance()->end();
    int gamedata = UserDefault::getInstance()->getIntegerForKey("game");
    gamedata++;
    UserDefault::getInstance()->setIntegerForKey("game", gamedata);
    UserDefault::getInstance()->flush();
    auto game=GameLevelLayer::createScene();
    Director::getInstance()->runWithScene(game);
}

void GameLevelLayer::toFlag() {
    log("win!");
    
    double leftTime = 300;
    if (UserDefault::getInstance()->getDoubleForKey("leftTime")) {
        leftTime = UserDefault::getInstance()->getDoubleForKey("leftTime");
    }
    double leftTime_s = leftTime - difftime(time(NULL), startTime);
    
    
    int score = UserDefault::getInstance()->getIntegerForKey("score");
    
    Point position = _player->getPosition();
    
    int addscore = (int)position.y * 2 + (int)leftTime_s * 2;
    score = score + addscore;
    UserDefault::getInstance()->setIntegerForKey("score", score);
    
    _player->removeFromParent();
    
    _player = Player::create("hold.png");
    
    _player->isPassed = true;
    
    _player->isAlive = true;
    
    _player->setPosition(position);
    
    this->addChild(_player);
    Sprite* flag = Sprite::create("flag.png");
    flag->setPosition(position);
    this->addChild(flag);
    
    auto staticbody = PhysicsBody::createBox(_player->getContentSize());
    
    _player->setPhysicsBody(staticbody);
    
    _player->getPhysicsBody()->setVelocity(Vec2::ZERO);
    
    
    
    
}
