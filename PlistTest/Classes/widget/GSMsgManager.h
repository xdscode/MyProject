//
//  GSMsgManager.h
//  PlistTest
//
//  Created by XD on 15/6/5.
//
//

#ifndef __PlistTest__GSMsgManager__
#define __PlistTest__GSMsgManager__

#include "cocos2d.h"
USING_NS_CC;
class GSMsgManager
{
private:
    GSMsgManager(){};
public:
    ~GSMsgManager(){};
    
    static GSMsgManager* s_instance;
    static GSMsgManager* getInstance();
private:
    __Array* m_pMsgArray;
public:
    void addMsg(const char* );
    void update();
    
};

#endif /* defined(__PlistTest__GSMsgManager__) */
