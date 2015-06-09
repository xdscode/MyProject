//
//  MoveLabel.h
//  PlistTest
//
//  Created by XD on 15/6/4.
//
//

#ifndef __PlistTest__MoveLabel__
#define __PlistTest__MoveLabel__
#include "cocos2d.h"
USING_NS_CC;

class MoveLabel : public Layer
{
public:
    MoveLabel(){};
    ~MoveLabel(){};
    bool init();
    CREATE_FUNC(MoveLabel);
    
    void addMsg(const char*);
    
};

#endif /* defined(__PlistTest__MoveLabel__) */
