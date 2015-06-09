//
//  GSVisibleRect.cpp
//  EmpireLegion
//
//  Created by empirefleet-carl on 4/30/14.
//
//

#include "GSVisibleRect.h"

// get design width & height & size & center point
Vec2 GSVisibleRect::getDesignCenter()
{
    Size designSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    return Vec2(designSize.width / 2, designSize.height / 2);
}

float GSVisibleRect::getDesignWidth()
{
    return Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width;
}

float GSVisibleRect::getDesignHeight()
{
    return Director::getInstance()->getOpenGLView()->getDesignResolutionSize().height;
}

Size GSVisibleRect::getDesignSize()
{
    return Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
}

// 针对不同机型的尺寸适配
Size GSVisibleRect::transSize(Size size)
{
    return GSVisibleRect::transSize(size.width, size.height);
}

Size GSVisibleRect::transSize(float width, float height)
{
    return Size(width, height + (GSVisibleRect::getDesignHeight()-960));
}

// 针对不同机型的坐标适配
Vec2 GSVisibleRect::transPoint(float x, float y, bool bCenter)
{
    if(bCenter)
        return Vec2(x, y + (GSVisibleRect::getDesignHeight()-960)/2);
    else
        return Vec2(x, y + (GSVisibleRect::getDesignHeight()-960));
}

Vec2 GSVisibleRect::transPoint(Vec2 point, bool bCenter)
{
    return GSVisibleRect::transPoint(point.x, point.y, bCenter);
}

// 图片全屏显示时的缩放比例
float GSVisibleRect::getResourceScale4FullScreen(float width, float height)
{
    return fmaxf(GSVisibleRect::getDesignWidth() / width, GSVisibleRect::getDesignHeight() / height);
}

float GSVisibleRect::getResourceScale4FullScreen(Size resSize)
{
    return fmaxf(GSVisibleRect::getDesignWidth() / resSize.width, GSVisibleRect::getDesignHeight() / resSize.height);
}
