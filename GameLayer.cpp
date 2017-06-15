#include"GameLayer.h"
#include<vector>

USING_NS_CC;

bool GameLevelLayer::init() {

	UserDefault::getInstance()->setIntegerForKey("score", 0);

	std::string path = UserDefault::getInstance()->getXMLFilePath();

	log("loaded:%s", path.c_str());

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("OnLand.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Jump.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("EatCoin.wav");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	screenWidth = visibleSize.width;
	screenHeight = visibleSize.height;

	map = TMXTiledMap::create("MarioMap1.tmx");
	map->setAnchorPoint(Vec2::ZERO);
	map->setPosition(Point::ZERO);
	float scale = screenHeight / (map->getMapSize().height*map->getTileSize().height);
	map->setScale(scale);
	this->addChild(map, -1);
	//加载地图

	SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wav", 1);
	_player = Player::create("smallJumpRight.png");
	_player->isAlive = true;
	_player->setPosition(20, 100);
	_player->setAnchorPoint(Vec2(0.5f, 0));
	_player->setScale(1);
	_player->setTag(1);



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

	Land = map->getLayer("land");

	int gid;
	for (int i = 0; i < Land->getLayerSize().width; i++) {
		gid = Land->getTileGIDAt(Vec2(i, 12));
		if (gid) {

			//(Land->getTileAt(Vec2(i, 12)))->setAnchorPoint(Vec2(0.5f, 1.0f));
			(Land->getTileAt(Vec2(i, 12)))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
				map->getTileSize().height)));
			(Land->getTileAt(Vec2(i, 12)))->getPhysicsBody()->setCategoryBitmask(0x01);
			(Land->getTileAt(Vec2(i, 12)))->getPhysicsBody()->setContactTestBitmask(0x01);
			(Land->getTileAt(Vec2(i, 12)))->getPhysicsBody()->setCollisionBitmask(0x01);
			(Land->getTileAt(Vec2(i, 12)))->getPhysicsBody()->setGroup(1);
		}
	}
	Pipe = map->getLayer("pipe");
	for (int i = 0; i < Pipe->getLayerSize().width; i++) {
		for (int j = 0; j < Pipe->getLayerSize().height; j++) {
			gid = Pipe->getTileGIDAt(Vec2(i, j));
			if (gid) {

				//Pipe->getTileAt(Vec2(i,j))->setAnchorPoint(Vec2(0.5f, 1.0f));
				Pipe->getTileAt(Vec2(i, j))->setPhysicsBody(PhysicsBody::createEdgeBox(Size(map->getTileSize().width,
					map->getTileSize().height)));
				Pipe->getTileAt(Vec2(i, j))->getPhysicsBody()->setCategoryBitmask(0x01);
				Pipe->getTileAt(Vec2(i, j))->getPhysicsBody()->setCollisionBitmask(0x01);
				Pipe->getTileAt(Vec2(i, j))->getPhysicsBody()->setContactTestBitmask(0x01);
				Pipe->getTileAt(Vec2(i, j))->getPhysicsBody()->setGroup(1);
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

	auto timerLayer = Layer::create();
	timerLabel = Label::create();
	timerLabel->setTextColor(Color4B::WHITE);
	timerLabel->setSystemFontSize(40);
	timerLabel->setAnchorPoint(Vec2(0, 1));
	timerLabel->setPosition(Vec2(20, visibleSize.height));
	timerLayer->addChild(timerLabel);
	startTime = time(NULL);
	schedule(schedule_selector(GameLevelLayer::timer));
	scheduleOnce(schedule_selector(GameLevelLayer::updateStart), 0);
	//声音按钮
	if (SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 1) {
		soundLabel = MenuItemLabel::create(Label::createWithSystemFont("声音:开", "Arial", 40), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
		soundLabel->setTag(11);
	}
	else {
		soundLabel = MenuItemLabel::create(Label::createWithSystemFont("声音:关", "Arial", 40), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
		soundLabel->setTag(10);
	}
	soundLabel->setAnchorPoint(Vec2(0.5, 1));
	soundLabel->setPositionX(visibleSize.width / 2);
	soundLabel->setColor(Color3B::WHITE);
	//返回按钮
	saveLabel = MenuItemLabel::create(Label::createWithSystemFont("返回", "Arial", 40), CC_CALLBACK_1(GameLevelLayer::menuCallBack, this));
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
	if (keycode == rightarrow) {
		smallWalkRight();
	}
	if (keycode == leftarrow) {
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

	//Point position = _player->getPosition();

	//log("%f,%f", position.x, position.y);
	int x = _player->getPositionX();
	if (x < screenWidth / 2) {
		x = screenWidth / 2;
	}
	if (x > map->getMapSize().width*map->getTileSize().width - screenWidth / 2) {
		x = map->getMapSize().width*map->getTileSize().width - screenWidth / 2;
	}
	timerLayer->setPositionX(x - screenWidth / 2);

	auto rightarrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto leftarrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	auto uparrow = EventKeyboard::KeyCode::KEY_UP_ARROW;
	auto downarrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(rightarrow) && _player->isAlive == true) {
		keyPressedDuration(rightarrow, delta);
	}
	if (isKeyPressed(leftarrow) && _player->isAlive == true) {
		keyPressedDuration(leftarrow, delta);
	}
	if (!isKeyPressed(rightarrow) && _player->getPhysicsBody()->getVelocity().x>0) {
		_player->getPhysicsBody()->setVelocity(Vect(0, _player->getPhysicsBody()->getVelocity().y));
	}
	if (!isKeyPressed(leftarrow) && _player->getPhysicsBody()->getVelocity().x<0) {
		_player->getPhysicsBody()->setVelocity(Vect(0, _player->getPhysicsBody()->getVelocity().y));
	}
	if (isKeyPressed(uparrow) && _player->isAlive == true) {
		keyPressedDuration(uparrow, delta);
	}

	if (_player->getPosition().y <= 0 && _player->isAlive == true) {

		_player->isAlive = false;

		playerdie();
	}

	Director::getInstance()->getRunningScene()->getPhysicsWorld()->step(1.0f / 180.0f);

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
			_player->getPhysicsBody()->applyImpulse(Vect(-3000, 0));
		}
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (_player->getPhysicsBody()->getVelocity().x < 1500) {
			_player->getPhysicsBody()->applyImpulse(Vect(3000, 0));
		}
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (_player->isOnGround) {
			SimpleAudioEngine::getInstance()->playEffect("Jump.wav");
			if (_player->getPhysicsBody()->getVelocity().y < 1000) {
				_player->getPhysicsBody()->applyImpulse(Vect(0, 80000));
				_player->isOnGround = false;
				log("jump");
			}

		}
		break;
	}
	setViewpointCenter(_player->getPosition());

}







void GameLevelLayer::setViewpointCenter(Vec2 position) {
	int x = MAX(position.x, screenWidth / 2);
	int y = 0;
	x = MIN(x, (map->getMapSize().width*map->getTileSize().width - screenWidth / 2));
	Point actualPosition = Vec2(x, y);
	Point center = Vec2(screenWidth / 2, 0);
	Point viewPoint = center - actualPosition;
	this->setPosition(viewPoint);
}



bool GameLevelLayer::onContactBegin(PhysicsContact& contact) {
	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	if (spriteA&&spriteB) {
		if (spriteA->getTag() == 1 && spriteB->getPhysicsBody()->getGroup() == 1 || spriteA->getPhysicsBody()->getGroup() == 1 && spriteB->getTag() == 1) {

			if (spriteB->getPhysicsBody()->getGroup() == 1) {

				if (_player->getPosition().y >= spriteB->getPosition().y) {

					_player->isOnGround = true;

				}

			}

			if (spriteA->getPhysicsBody()->getGroup() == 1) {

				if (_player->getPosition().y >= spriteA->getPosition().y) {

					_player->isOnGround = true;
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
			SimpleAudioEngine::getInstance()->playEffect("EatCoin.wav");

			int score = UserDefault::getInstance()->getIntegerForKey("score");

			score = score + 100;

			UserDefault::getInstance()->setIntegerForKey("score", score);

			UserDefault::getInstance()->flush();

			log("%d", score);
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



Vec2 GameLevelLayer::tileCoordForPosition(const Vec2& position) {
	Size mapsize = map->getMapSize();
	Size tilesize = map->getTileSize();
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
	while (!_player->isOnGround) {
		_player->initWithFile("smallJumpRight.png");
	}
	if (keys[rightarrow]) {
		smallWalkRight();
	}
	if (keys[leftarrow]) {
		smallWalkLeft();
	}
}



void GameLevelLayer::playerdie() {
	_player->stopAllActions();
	_player->initWithFile("die.png");
	_player->getPhysicsBody()->setVelocity(Vec2::ZERO);
	_player->getPhysicsBody()->setCategoryBitmask(0x02);
	_player->getPhysicsBody()->setContactTestBitmask(0x02);
	_player->getPhysicsBody()->setCollisionBitmask(0x02);
	_player->getPhysicsBody()->setVelocity(Vec2(0, 1500));
	int score = UserDefault::getInstance()->getIntegerForKey("score");
	log("%d", score);
}



void GameLevelLayer::timer(float dt) {
	double usedTime_s = 300 - difftime(time(NULL), startTime);
	if (usedTime_s >= 0) {
		double usedTime_min = usedTime_s / 60;
		timerLabel->setString(StringUtils::format("%02d:%02d", (int)usedTime_min, (int)(usedTime_s) % 60));
	}



	else {
		unschedule(schedule_selector(GameLevelLayer::timer));
		//执行游戏结束的函数
		playerdie();
		toEndingScene(false);
	}



}

void GameLevelLayer::updateStart(float dt) {

	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setAutoStep(false);
	scheduleUpdate();
}



Scene* GameLevelLayer::createScene() {

	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setGravity(Vect(0.0f, -15000.0f));

	auto layer = GameLevelLayer::create();

	scene->addChild(layer);

	return scene;

}



void GameLevelLayer::menuCallBack(Ref* pSender) {

	switch (((MenuItemImage *)pSender)->getTag()) {
	case 10:
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		soundLabel->setString("声音:开");
		soundLabel->setTag(11);
		break;
	case 11:
		SimpleAudioEngine::getInstance()->pauseAllEffects();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		soundLabel->setString("声音:关");
		soundLabel->setTag(10);
		break;

	case 20:
		//存储游戏进度，未完成
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->stopAllEffects();
		toMainScene();
		break;
	default:
		break;
	}

}



void GameLevelLayer::toEndingScene(const bool& isPass) {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->stopAllEffects();
	auto scene = Scene::create();
	auto layer = endingScene::create();
	layer->passTheGame = isPass;
	layer->addTitle();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);



}



void GameLevelLayer::toMainScene() {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->stopAllEffects();
	auto scene = MenuLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}
