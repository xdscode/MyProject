//
//  GSExpanMenu.h
//  PlistTest
//
//  Created by XD on 15/5/25.
//
//

#ifndef __PlistTest__GSExpanMenu__
#define __PlistTest__GSExpanMenu__
#include "cocos2d.h"

USING_NS_CC;

class ExpanDelegate
{
public:
    ExpanDelegate(){};
    ~ExpanDelegate(){};
    virtual void onCallBack1(Ref* v1)=0;
    virtual void onCallBack2(Ref* v1)=0;
    virtual void onCallBack3(Ref* v1)=0;
};

class GSExpanMenu : public Layer
{
public:
    GSExpanMenu();
    ~GSExpanMenu();
    bool init();
    CREATE_FUNC(GSExpanMenu);
    static GSExpanMenu* createMenu(int tab);
    void initMenu(int tab);
    void _move(Ref* v1);
    void _reset(Ref*);
    void _removeSelf(Ref* v1);
    bool onTouchBegan(Touch *touch, Event *unused_event);
    
public:
    void tempCC1(Ref*);
    void tempCC2(Ref*);
    void tempCC3(Ref*);
    void setDelegate(ExpanDelegate* delegate)
    {
        _delegate = delegate;
    };
    
private:
    void _expan(Ref* v1,Ref* v2,Ref* v3);
    Menu*                   _rootMenu;
    int                     _tab;
    ExpanDelegate*          _delegate;
};

#endif /* defined(__PlistTest__GSExpanMenu__) */
