//
//  GSMsgManager.cpp
//  PlistTest
//
//  Created by XD on 15/6/5.
//
//

#include "GSMsgManager.h"
GSMsgManager* GSMsgManager::s_instance = NULL;

GSMsgManager* GSMsgManager::getInstance()
{
    if (!s_instance)
    {
        s_instance = new GSMsgManager();
    }
    return s_instance;
}

void GSMsgManager::addMsg(const char * ss)
{
    __String* str = __String::create(ss);
    m_pMsgArray->addObject(str);
}

void GSMsgManager::update()
{
    
}