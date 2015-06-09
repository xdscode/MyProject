//
//  CCBlockMenu.cpp
//  seawar-v2.0.4
//  事件拦截menu
//  Created by gaodan on 12-12-3.
//
//

#include "CCBlockMenu.h"

CCBlockMenu::CCBlockMenu(void)
{
    
}
CCBlockMenu::~CCBlockMenu(void)
{
    
}

CCBlockMenu* CCBlockMenu::create()
{
    return CCBlockMenu::create(NULL, NULL);
}

CCBlockMenu * CCBlockMenu::create(MenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    CCBlockMenu *pRet = new CCBlockMenu();
    
    Vector<MenuItem*> pArray;
    if( item )
    {
        MenuItem* item = va_arg(args, MenuItem*);
        while(item)
        {
            pArray.pushBack(item);
            item = va_arg(args, MenuItem*);
        }
    }
    
    if (pRet && pRet->initWithArray(pArray))
    {
        pRet->autorelease();
        va_end(args);
        return pRet;
    }
    va_end(args);
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCBlockMenu* CCBlockMenu::createWithArray(const Vector<MenuItem*>& arrayItems)
{
    CCBlockMenu* pRet = new CCBlockMenu();
    if (pRet && pRet->initWithArray(arrayItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

CCBlockMenu* CCBlockMenu::createWithItem(MenuItem* item)
{
    return CCBlockMenu::create(item, NULL);
}

CCBlockMenu* CCBlockMenu::createWithItems(MenuItem* item, va_list args)
{
    Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    
    return CCBlockMenu::createWithArray(items);
}

bool CCBlockMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
        
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(Vec2(s.width/2, s.height/2));
        
        int z=0;
        
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
        
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        
        EventListenerTouchOneByOne* m_touchListener = EventListenerTouchOneByOne::create();
        m_touchListener->setSwallowTouches(true);
        
        m_touchListener->onTouchBegan = CC_CALLBACK_2(CCBlockMenu::onTouchBegan, this);
        m_touchListener->onTouchMoved = CC_CALLBACK_2(CCBlockMenu::onTouchMoved, this);
        m_touchListener->onTouchEnded = CC_CALLBACK_2(CCBlockMenu::onTouchEnded, this);
        m_touchListener->onTouchCancelled = CC_CALLBACK_2(CCBlockMenu::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
        
        return true;
    }
    return false;
}

bool CCBlockMenu::onTouchBegan(Touch* touch, Event* event)
{
    if(!isVisible()) return false;
    for (Node *c = this->_parent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    return true;// 始终返回true接收并拦截所有touch事件
}

void CCBlockMenu::onTouchEnded(Touch* touch, Event* event)
{
    
}

void CCBlockMenu::onTouchCancelled(Touch *touch, Event* event)
{
    
}

void CCBlockMenu::onTouchMoved(Touch* touch, Event* event)
{
    
}
