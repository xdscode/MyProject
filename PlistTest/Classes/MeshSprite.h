//
//  MeshSprite.h
//  EmpireLegion3.0
//
//  Created by gamesoul-empirelegion on 15-2-10.
//
//

#ifndef __EmpireLegion3_0__MeshSprite__
#define __EmpireLegion3_0__MeshSprite__


#include "cocos2d.h"
#include "ExtensionMacros.h"


USING_NS_CC;



class CC_DLL MeshSprite : public Sprite
{
    
public:
    
    static MeshSprite* create();
    
    void setMeshPoints(Vec2 p1,Vec2 p2,Vec2 p3,Vec2 p4);
    
    virtual void setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize);
    virtual void updateTransform(void);
    
    virtual bool initWithSpriteFrameName(const std::string& spriteFrameName);
    
public:
    
    float x1,x2,x3,x4;
    float y1,y2,y3,y4;
    
};








#endif /* defined(__EmpireLegion3_0__MeshSprite__) */
