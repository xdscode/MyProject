//
//  GSMoveLabel.h
//  PlistTest
//
//  Created by XD on 15/6/1.
//
//

#ifndef __PlistTest__GSMoveLabel__
#define __PlistTest__GSMoveLabel__
#include "cocos2d.h"
USING_NS_CC;
class GSMoveLabel : public Label
{
public:
    GSMoveLabel(){};
    ~GSMoveLabel(){};
    
    bool init();
    CREATE_FUNC(GSMoveLabel);
};

#endif /* defined(__PlistTest__GSMoveLabel__) */
