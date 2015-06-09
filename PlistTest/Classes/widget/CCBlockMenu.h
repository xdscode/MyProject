//
//  CCBlockMenu.h
//  seawar-v2.0.4
//  事件拦截menu
//  Created by gaodan on 12-12-3.
//
//

#ifndef __seawar_v2_0_4__CCBlockMenu__
#define __seawar_v2_0_4__CCBlockMenu__

#include "cocos2d.h"

using namespace cocos2d;

class CCBlockMenu : public Menu
{
public:
    CCBlockMenu(void);
    virtual ~CCBlockMenu(void);
    
    /** creates an empty CCScrollMenu */
    static CCBlockMenu* create();
    
    /** creates a CCScrollMenu with it's items */
    static CCBlockMenu* create(MenuItem* item, ...);
    
    // creates a CCScrollMenu with a CCArray of CCMenuItem objects
    static CCBlockMenu* createWithArray(const Vector<MenuItem*>& arrayItems);
    
    /** creates a CCScrollMenu with it's item, then use addChild() to add
     * other items. It is used for script, it can't init with undetermined
     * number of variables.
     */
    static CCBlockMenu* createWithItem(MenuItem* item);
    /** creates a Menu with MenuItem objects */
    static CCBlockMenu* createWithItems(MenuItem *firstItem, va_list args);
    /** 覆盖Menu的同名方法 */
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
    
    /**
     @brief For phone event handle functions
     */
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch *touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    
private:
    
};

#endif
