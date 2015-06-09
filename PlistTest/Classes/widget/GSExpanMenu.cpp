//
//  GSExpanMenu.cpp
//  PlistTest
//
//  Created by XD on 15/5/25.
//
//

#include "GSExpanMenu.h"
#include "CCBlockMenu.h"
#include "GSVisibleRect.h"
GSExpanMenu::GSExpanMenu()
{
    _rootMenu = Menu::create();
    _rootMenu->setAnchorPoint(Vec2::ZERO);
    _rootMenu->setPosition(Vec2::ZERO);
    _rootMenu->retain();
    this->addChild(_rootMenu);
}

GSExpanMenu::~GSExpanMenu()
{
    CC_SAFE_RELEASE_NULL(_rootMenu);
}

bool GSExpanMenu::init()
{
    return Layer::init();
}

void GSExpanMenu::initMenu(int tab)
{
    _tab=tab;
    // 创建一个全屏按钮
    LayerColor* node = LayerColor::create(Color4B(0,0,0,0), GSVisibleRect::getDesignWidth()*2, GSVisibleRect::getDesignHeight()*2);
    MenuItemSprite* item = MenuItemSprite::create(node,node);
    item->setCallback(CC_CALLBACK_1(GSExpanMenu::_removeSelf, this));
//    Vec2 v2(this->convertToWorldSpace(Vec2::ZERO));
//    item->setAnchorPoint(Vec2::ZERO);
//    item->setPosition(Vec2(-v2.x,-v2.y));
    Menu* blockMenu = Menu::create(item,NULL);
    this->addChild(blockMenu,-100);
    for (int i=1; i<=tab; ++i)
    {
        char buf[200];
        sprintf(buf, "icon_bldg_%d.png",i);
        MenuItemSprite* item = MenuItemSprite::create(Sprite::createWithSpriteFrameName(buf), Sprite::createWithSpriteFrameName(buf));
        item->setTag(i);
        if(i==1)
            item->setCallback(CC_CALLBACK_1(GSExpanMenu::tempCC1, this));
        else if(i==2)
            item->setCallback(CC_CALLBACK_1(GSExpanMenu::tempCC2, this));
        else if(i==3)
            item->setCallback(CC_CALLBACK_1(GSExpanMenu::tempCC3, this));
        float x=150*cosf(3.14* (180-30.0f*(i+1))/180);
        float y=150*sinf(3.14* (180-30.0f*(i+1))/180);
        auto* action = CCMoveBy::create(0.2f, Vec2(x,y));
        EaseBackIn* acbk = EaseBackIn::create(action);
        //        auto* speed = CCSpeed::create(action, 1.5f);
        item->runAction(acbk);
        
        _rootMenu->addChild(item);
    }
}

GSExpanMenu* GSExpanMenu::createMenu(int tab)
{
    GSExpanMenu* GSExpanMenu = GSExpanMenu::create();
    GSExpanMenu->initMenu(tab);
    return GSExpanMenu;
}

void GSExpanMenu::_expan(Ref* v1,Ref* v2,Ref* v3)
{
    
}

void GSExpanMenu::_move(Ref* v1)
{
    for(int i=1;i<=_tab;++i)
    {
        Node* node = _rootMenu->getChildByTag(i);
        float y=100*sinf(3.14* (180-30.0f*(i+1))/180);
        float x=100*cosf(3.14* (180-30.0f*(i+1))/180);
        auto* action = CCMoveBy::create(0.5, Vec2(x,y));
        auto* speed = CCSpeed::create(action, 1.5f);
        node->runAction(speed);
    }
}

void GSExpanMenu::_reset(Ref*)
{
    for (int i=1; i<=_tab; ++i)
    {
        Node* node = _rootMenu->getChildByTag(i);
        node->setPosition(Vec2::ZERO);
    }
}


void GSExpanMenu::tempCC1(Ref* pSender)
{
    _delegate->onCallBack1(pSender);
}

void GSExpanMenu::tempCC2(Ref* pSender)
{
    _delegate->onCallBack2(pSender);
}

void GSExpanMenu::tempCC3(Ref* pSender)
{
    _delegate->onCallBack3(pSender);
}

void GSExpanMenu::_removeSelf(Ref* v1)
{
//    this->removeFromParentAndCleanup(true);
}

bool GSExpanMenu::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    return false;
}