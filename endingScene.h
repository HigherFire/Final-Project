#ifndef __ENDING_SCENE_H__
#define __ENDING_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class endingScene : public cocos2d::Layer
{
public:
    bool passTheGame = false;

    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(endingScene);
    
    void endingCallBack(Ref* pSender);
    
    
};

#endif // __ENDING_SCENE_H__
