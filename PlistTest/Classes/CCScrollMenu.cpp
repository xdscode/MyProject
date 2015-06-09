//
//  CCScrollMenu.cpp
//  FoxUGame
//  主要处理cctouchBegin时的开始point点的记录，cctouchMoved时的距离判定并取消menuitem的选中状态，在注册触摸监听的时候，让menu允许事件穿透。
//  Created by gaodan on 12-8-20.
//  Copyright (c) 2012年 FoxUGame. All rights reserved.
//

#include "CCScrollMenu.h"

// 判断是否是点击事件(可拖动)的距离阀值
const int OFFSET_VALUE_THRESHOLD                    = 5;

bool CCScrollMenu::m_isMoved = true;
void CCScrollMenu::setIsMoved(bool flag)
{
    m_isMoved = flag;
}

bool CCScrollMenu::isMoved()
{
    return m_isMoved;
}

CCScrollMenu::CCScrollMenu(void)
: m_bIsRevertTouchPriority(false),m_touchListener(NULL)
{
    m_iHandlePriority = 0;
    m_uDisable_direction = SCROLL_MENU_DIRECTION_BOTH;
}

CCScrollMenu::~CCScrollMenu(void)
{
    
}

CCScrollMenu* CCScrollMenu::create()
{
    return CCScrollMenu::create(NULL, NULL);
}

CCScrollMenu* CCScrollMenu::create(MenuItem* item, ...)
{
    va_list args;
    va_start(args, item);
    CCScrollMenu* pRet = new CCScrollMenu();
    
    Vector<MenuItem*> arrayItems;
    if(item)
    {
        arrayItems.pushBack(item);
        MenuItem* i = va_arg(args, MenuItem*);
        while(i)
        {
            arrayItems.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    
    if (pRet && pRet->initWithArray(arrayItems))
    {
        pRet->autorelease();
        va_end(args);
        return pRet;
    }
    
    va_end(args);
    CC_SAFE_DELETE(pRet);
    return NULL;
}


CCScrollMenu* CCScrollMenu::create(Menu* menu)
{
    CCScrollMenu* pRet = new CCScrollMenu();
    Vector<MenuItem*> arrayItems;
    for(auto item:menu->getChildren())
    {
        MenuItem* temp=dynamic_cast<MenuItem*>(item);
        if(temp)
        {
            arrayItems.pushBack(temp);
        }
    }
    for(auto item:arrayItems)
    {
        item->removeFromParent();
    }
    
    if (pRet && pRet->initWithArray(arrayItems))
    {
        pRet->setPosition(menu->getPosition());
        pRet->setAnchorPoint(menu->getAnchorPoint());
        pRet->setTag(menu->getTag());
        pRet->setContentSize(menu->getContentSize());
        pRet->setScale(menu->getScaleX(), menu->getScaleY());
        
        pRet->autorelease();
        return pRet;
    }
    return NULL;
    
}


void CCScrollMenu::replaceMenu(Node* node)
{
    for(auto item:node->getChildren())
    {
        Menu *temp=dynamic_cast<Menu*>(item);
        if(temp)
        {
            CC_SAFE_RETAIN(temp);
            Node* par=temp->getParent();
            temp->removeFromParent();
            CCScrollMenu *mm=CCScrollMenu::create(temp);
            par->addChild(mm,-1);
            CC_SAFE_RELEASE(temp);
        }
        {
            replaceMenu(item);
        }
    }
    
}


CCScrollMenu* CCScrollMenu::createWithArray(const Vector<MenuItem*>& arrayItems)
{
    CCScrollMenu* pRet = new CCScrollMenu();
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

CCScrollMenu* CCScrollMenu::createWithItem(MenuItem* item)
{
    return CCScrollMenu::create(item, NULL);
}

CCScrollMenu* CCScrollMenu::createWithItems(MenuItem* item, va_list args)
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
    
    return CCScrollMenu::createWithArray(items);
}

bool CCScrollMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
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
        
        
        m_touchListener = EventListenerTouchOneByOne::create();
        m_touchListener->setSwallowTouches(false);
        
        m_touchListener->onTouchBegan = CC_CALLBACK_2(CCScrollMenu::onTouchBegan, this);
        m_touchListener->onTouchMoved = CC_CALLBACK_2(CCScrollMenu::onTouchMoved, this);
        m_touchListener->onTouchEnded = CC_CALLBACK_2(CCScrollMenu::onTouchEnded, this);
        m_touchListener->onTouchCancelled = CC_CALLBACK_2(CCScrollMenu::onTouchCancelled, this);
        
//        m_touchListener->targetPaused = CC_CALLBACK_0(Menu::targetPaused, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
        
        return true;
    }
    return false;
}

bool CCScrollMenu::onTouchBegan(Touch* touch, Event* event)
{
    Point touchLocation = touch->getLocationInView();
    Point point= Director::getInstance()->convertToGL(touchLocation);
    m_TouchPoint.setPoint(point.x, point.y);
    return Menu::onTouchBegan(touch, event);
}

void CCScrollMenu::onTouchMoved(Touch* touch, Event* event)
{
    if(!m_isMoved)
    {
        Menu::onTouchMoved(touch, event);
        return;
    }
    
    Point touchLocation = touch->getLocationInView();
    Point point = Director::getInstance()->convertToGL(touchLocation);
    if(_selectedItem)
    {
        Rect rect = _selectedItem->rect();
        rect.origin.x = 0;
        rect.origin.y = 0;
        if(!rect.containsPoint(_selectedItem->convertToNodeSpace(point)))// 超出按钮范围就取消按钮选中状态
        {
            _selectedItem->unselected();
            _selectedItem = NULL;
            return;
        }
    }
    
    float offsetX = fabsf(point.x - m_TouchPoint.x);
    float offsetY = fabsf(point.y - m_TouchPoint.y);
    if((m_uDisable_direction == SCROLL_MENU_DIRECTION_BOTH && (offsetX > OFFSET_VALUE_THRESHOLD || offsetY > OFFSET_VALUE_THRESHOLD)) ||
       (m_uDisable_direction == SCROLL_MENU_DIRECTION_H && (offsetX > OFFSET_VALUE_THRESHOLD)) ||
       (m_uDisable_direction == SCROLL_MENU_DIRECTION_V && (offsetY > OFFSET_VALUE_THRESHOLD)))// 如果横向或者纵向的移动距离大于offset_value阀值就取消menu菜单响应
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
            _selectedItem = NULL;
        }
    }
}

void CCScrollMenu::onTouchEnded(Touch* touch, Event* event)
{
    Menu::onTouchEnded(touch, event);
}

void CCScrollMenu::onTouchCancelled(Touch* touch, Event* event)
{
    Menu::onTouchCancelled(touch, event);
}

void CCScrollMenu::setSwallowTouches(bool isSwallow)
{
    if(m_touchListener)
    {
        m_touchListener->setSwallowTouches(isSwallow);
    }
}

MenuItem* CCScrollMenu::itemForTouch(Touch *touch)
{
    if(!m_bIsRevertTouchPriority)
    {
        return Menu::getItemForTouch(touch);
    }
    
    Point touchLocation = touch->getLocation();
    
    if (_children.size() > 0)
    {
        auto nodeIter = _children.begin();
        while (nodeIter != _children.end())
        {
            MenuItem* pChild = dynamic_cast<MenuItem*>(*nodeIter);
            if (pChild && pChild->isVisible() && pChild->isEnabled())
            {
                Point local = pChild->convertToNodeSpace(touchLocation);
                Rect r = pChild->rect();
                r.origin = Vec2(0,0);
                
                if (r.containsPoint(local))
                {
                    return pChild;
                }
            }
        }
    }
    
    return NULL;
}
