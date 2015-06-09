//
//  GSVisibleRect.h
//  EmpireLegion
//
//  Created by empirefleet-carl on 4/30/14.
//
//

#ifndef EmpireLegion_VisibleRect_h
#define EmpireLegion_VisibleRect_h

#include "cocos2d.h"

USING_NS_CC;

class GSVisibleRect {
public:
    // design width & height & size & center point
    static Size getDesignSize();
    static float getDesignWidth();
    static float getDesignHeight();
    static Vec2 getDesignCenter();

    // 针对不同机型的尺寸适配
    static Size transSize(Size size);
    static Size transSize(float width, float height);
    
    // 针对不同机型的坐标适配
    static Vec2 transPoint(Vec2 point, bool bCenter);
    static Vec2 transPoint(float x, float y, bool bCenter);
    
    // 图片全屏显示时的缩放比例
    static float getResourceScale4FullScreen(float width, float height);
    static float getResourceScale4FullScreen(Size resSize);
};

#endif
