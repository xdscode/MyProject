//
//  CCScrollMenu.h
//  FoxUGame
//
//  Created by gaodan on 12-8-20.
//  Copyright (c) 2012年 FoxUGame. All rights reserved.
//

#ifndef FoxUGame_CCScrollMenu_h
#define FoxUGame_CCScrollMenu_h

#include "cocos2d.h"

USING_NS_CC;

class CCScrollMenu : public Menu
{
public:
    CCScrollMenu();
    virtual ~CCScrollMenu();
    
    enum {
        SCROLL_MENU_DIRECTION_BOTH,
        SCROLL_MENU_DIRECTION_H,
        SCROLL_MENU_DIRECTION_V
    };
    
    // creates an empty CCScrollMenu
    static CCScrollMenu* create();
    
    // creates a CCScrollMenu with it's items
    static CCScrollMenu* create(MenuItem* item, ...);
    
    // creates a CCScrollMenu with it's ccmenu
    static CCScrollMenu* create(Menu* menu);
    
    // creates a CCScrollMenu with a CCArray of CCMenuItem objects
    static CCScrollMenu* createWithArray(const Vector<MenuItem*>& arrayItems);
    
    /** creates a CCScrollMenu with it's item, then use addChild() to add 
     * other items. It is used for script, it can't init with undetermined
     * number of variables.
     */
    static CCScrollMenu* createWithItem(MenuItem* item);
    /** creates a Menu with MenuItem objects */
    static CCScrollMenu* createWithItems(MenuItem *firstItem, va_list args);
    /** 覆盖Menu的同名方法 */
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
    //将ccmenu替换成ccscrollmenu
    static void replaceMenu(Node* node);
    
    
    static bool isMoved();
    static void setIsMoved(bool flag);
    
    /**
     @brief For phone event handle functions
     */
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
    // 设置m_touchListener是否吞噬触摸事件
    void setSwallowTouches(bool isSwallow);
    
    // 是否翻转子节点触摸优先级
    CC_SYNTHESIZE(bool, m_bIsRevertTouchPriority, IsRevertTouchPriority);
    
    // 触摸事件优先级
    CC_SYNTHESIZE(int, m_iHandlePriority, HandlePriority);
    
    // menu菜单滑动禁止menuitem生效的方向
    CC_SYNTHESIZE(unsigned int, m_uDisable_direction, DisableDirection);
    
protected:
    MenuItem* itemForTouch(Touch * touch);
    
private:
    Vec2                                m_TouchPoint;
    static bool                         m_isMoved;
    EventListenerTouchOneByOne*         m_touchListener;
};

#endif
