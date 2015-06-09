#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "external/Box2D/Box2D.h"
#include "MeshSprite.h"
#include "GSExpanMenu.h"
#define BILI 48.0f
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer ,public ExpanDelegate
{
public:
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    void onCallBack(Ref* );
    void onReset(Ref*);
    virtual void onCallBack1(Ref* v1);
    virtual void onCallBack2(Ref* v1);
    virtual void onCallBack3(Ref* v1);
    void move(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
public:
    virtual void update(float dt);
    MeshSprite*             m_pMeshSprite;
    b2World* world;
    cocos2d::Sprite* bird;
    
private:
    void addBird();
    void initWorld();
};

#endif // __HELLOWORLD_SCENE_H__
