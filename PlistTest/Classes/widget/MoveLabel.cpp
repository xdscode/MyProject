//
//  MoveLabel.cpp
//  PlistTest
//
//  Created by XD on 15/6/4.
//
//

#include "MoveLabel.h"
#include "cocos2d.h"
#include "UIScale9Sprite.h"

bool MoveLabel::init()
{
    return Layer::init();
}

void MoveLabel::addMsg(const char * msg)
{
//    DrawNode* drawNode = DrawNode::create();
//    drawNode->drawSolidRect(Vec2(200,400), Vec2(600,800), Color4F(1,1,1,1));
//    Size size = Director::getInstance()->getOpenGLView()-> getDesignResolutionSize();
//    Sprite* sprite = Sprite::create("HelloWorld.png");
//    sprite->setScale(640/sprite->getContentSize().width, 1136/sprite->getContentSize().height);
//    sprite->setPosition(Vec2::ZERO);
//    sprite->setAnchorPoint(Vec2::ZERO);
//    cocos2d::ui::Scale9Sprite* scaleSPrite = cocos2d::ui::Scale9Sprite::create("ui_prompt_border.png");
//    scaleSPrite->setAnchorPoint(Vec2(0.5,0.5));
//    scaleSPrite->setPosition(size.width/2,size.height/2);
//    //clipping node
//    ClippingNode* clipNode = ClippingNode::create();
//    clipNode->setAlphaThreshold(0.093f);
//    clipNode->setContentSize(size);
//    clipNode->setPosition(Vec2::ZERO);
//    clipNode->setAnchorPoint(Vec2::ZERO);
//    clipNode->setStencil(scaleSPrite);
//    clipNode->addChild(sprite);
//    
    
    //上面都是扯犊子的
    Size size = Director::getInstance()->getOpenGLView()-> getDesignResolutionSize();
    ClippingNode* clipNode = ClippingNode::create();
    clipNode->setContentSize(size);
    clipNode->setPosition(Vec2::ZERO);
    clipNode->setAnchorPoint(Vec2::ZERO);
    
    ui::Scale9Sprite* scaleSprite = ui::Scale9Sprite::create("ui_prompt_border.png");
    scaleSprite->setContentSize(Size(500,60));
    scaleSprite->setAnchorPoint(Vec2(0.5f,0.5));
    scaleSprite->setPosition(size.width/2,size.height-200);
    clipNode->setStencil(scaleSprite);
    
    Label* label = Label::createWithSystemFont(msg, "", 35);
    label->setPosition(size.width-100,size.height-225);
    label->setAnchorPoint(Vec2::ZERO);
    clipNode->addChild(label);
    
    auto action = MoveBy::create(20.0f, Vec2(-1640,0));
    label->runAction(action);
    
    ui::Scale9Sprite* scalebgSprite = ui::Scale9Sprite::create("ui_prompt_border.png");
    scalebgSprite->setContentSize(Size(500,60));
    scalebgSprite->setAnchorPoint(Vec2(0.5f,0.5));
    scalebgSprite->setPosition(size.width/2,size.height-200);
    Node* node = Node::create();
    node->addChild(scalebgSprite);
    node->addChild(clipNode);
    
    Director::getInstance()->getRunningScene()->addChild(node);
}