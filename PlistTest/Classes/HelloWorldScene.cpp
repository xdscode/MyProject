
#include "HelloWorldScene.h"
#include  "TableButon.h"
#include  "MoveLabel.h"
USING_NS_CC;

HelloWorld::HelloWorld():world(new b2World(b2Vec2(0,-10)))
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runbefore.plist","runbefore.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gs_ui_common_a81.plist","gs_ui_common_a81.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gs_icon_building.plist","gs_icon_building.pvr.ccz");
    m_pMeshSprite = NULL;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
//    EventListenerTouchOneByOne* eventListener = EventListenerTouchOneByOne::create();
//    eventListener->setSwallowTouches(true);
//    eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//    eventListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//    eventListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
//    eventListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
//    _eventDispatcher->addEventListenerWithFixedPriority(eventListener,-1123);
    auto* bg = Sprite::create("HelloWorld.png");
    bg->setScale(2);
    bg->setPosition(Vec2(320,500));
    this->addChild(bg);
    auto* menuItem = MenuItemSprite::create(Sprite::create("CloseNormal.png"), Sprite::create("CloseSelected.png"),CC_CALLBACK_1(HelloWorld::onCallBack, this) );
    Vec2 position = Director::getInstance()->getVisibleSize();
    auto* menu = Menu::create(menuItem, NULL);
    menu->setPosition(50,position.y/2);
    this->addChild(menu);
    
    
    auto* menuItem1 = MenuItemSprite::create(Sprite::create("CloseNormal.png"), Sprite::create("CloseSelected.png"),CC_CALLBACK_1(HelloWorld::onReset, this) );
    auto* menu1 = Menu::create(menuItem1, NULL);
    menu1->setPosition(50,position.y/2-100);
    this->addChild(menu1);
//    m_pMeshSprite = MeshSprite::create();
//    m_pMeshSprite->initWithSpriteFrameName("RUNEASE0000");
//    m_pMeshSprite->setPosition(Vec2(400,100));
//    this->addChild(m_pMeshSprite,11);
//    
//    TableButon* buton = TableButon::createWithTable(nullptr);
//    buton->setPosition(Vec2(100,100));
//    this->addChild(buton,9999);
    
//    GSExpanMenu* mm = GSExpanMenu::createMenu(3);
//    mm->setDelegate(this);
//    mm->ignoreAnchorPointForPosition(false);
//    mm->setAnchorPoint(Vec2::ZERO);
//    mm->setPosition(Vec2(position.x/2,position.y/2-200));
//    this->addChild(mm,3,111);
    
    
//    this->schedule(schedule_selector(HelloWorld::move),1,CC_REPEAT_FOREVER,0);
    
//    Label* label = Label::createWithSystemFont("这是一个中英文 hunzade zifuchuan","", 50);
//    TTFConfig fg(label->getTTFConfig());
//    label->getString();
//    label->setDimensions(550, 0);
//    label->enableOutline(Color4B(255,0,0,125));
//    label->setAnchorPoint(Vec2::ZERO);
//    label->setPosition(Vec2(60,220));
//    this->addChild(label,99,99);
//    auto no = DrawNode::create();
//    no->drawLine(Vec2(60,200), Vec2(550,200),Color4F(1.0,0,0,1.0));
//    this->addChild(no);
    
    return true;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    m_pMeshSprite->setMeshPoints(Vec2(0,0),
                                 Vec2(10,0),
                                 Vec2(0,10),
                                 Vec2(10,10)
                                 );
    return false;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

}
void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void HelloWorld::update(float dt)
{
    world->Step (dt,8,3);
}
void HelloWorld::addBird()
{
    bird=Sprite::createWithSpriteFrameName("RUNEASE0000");
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    Size size=Director::getInstance()->getVisibleSize();
    bodyDef.position = b2Vec2(size.width/2/BILI,9);
    b2Body *birdBody=world->CreateBody (&bodyDef);
    birdBody->SetUserData(bird);
    this->addChild(bird);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onCallBack(Ref* pSender )
{
//    GSExpanMenu* mm = (GSExpanMenu*)this->getChildByTag(111);
//    mm->_move(NULL);
    
//    Label* label = dynamic_cast<Label* >(this->getChildByTag(99));
//    float scale = label->getScale()+0.3;
//    label->setScale(scale);
    
    MoveLabel* label = MoveLabel::create();
    label->addMsg("zasdfagz 这是中英文字符串asdf 的发的是大师傅阿斯顿发送到阿斯顿飞");
}

void HelloWorld::onReset(Ref*)
{
    GSExpanMenu* mm = (GSExpanMenu*)this->getChildByTag(111);
    mm->_reset(NULL);
}
void HelloWorld::onCallBack1(Ref* v1)
{

}

void HelloWorld::onCallBack2(Ref* v1)
{

}

void HelloWorld::onCallBack3(Ref* v1)
{

}

void HelloWorld::move(float dt)
{
    Label* label = dynamic_cast<Label* >(this->getChildByTag(99));
    Sprite* sprite = (Sprite*)label->getLetter(0);
//    sprite->setVisible(false);
}