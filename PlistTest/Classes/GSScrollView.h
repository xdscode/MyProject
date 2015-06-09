//
//  GSScrollView.h
//  FoxUGame
//  滚动视口
//  Created by gaodan on 12-10-24.
//
//

#ifndef __FoxUGame__ScrollView__
#define __FoxUGame__ScrollView__

#include "cocos2d.h"
#include "CCScrollView.h"
#include "CCScrollMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class GSScrollView : public ScrollView
{
public:
    GSScrollView(void);
    virtual ~GSScrollView(void);
    
    enum
    {
        MOVE_OFFSET_VALUE=5,// 移动距离阀值
    };
    
    static int moveOffsetValue;
    static bool isMoved(){return m_isMoved;};
    static void setIsMoved(bool flag);
    
    inline void setNonstaticIsMoved(bool flag){m_nonstaticIsMoved = flag;}
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static GSScrollView* create(Size size, Node* container = NULL);
    /**
     * Returns an autoreleased scroll view object.
     *
     * @param size view size
     * @param container parent object
     * @return autoreleased scroll view object
     */
    static GSScrollView* create();
    
    // 获取最大缩放比例
    inline float getMaxScale(){return _maxScale;}
    // 设置最大缩放比例
    inline void setMaxScale(float maxScale){_maxScale=maxScale;}
    // 获取最小缩放比例
    inline float getMinScale(){return _minScale;}
    // 设置最小缩放比例
    inline void setMinScale(float minScale){_minScale=minScale;}
    // 获取边距的最大超出范围比例
    inline float getInsetRatio(){return m_fInsetRatio;}
    // 设置边距的最大超出范围比例
    inline void setInsetRatio(float ratio){m_fInsetRatio=ratio;}
    
    // 递归检测node节点上的所有子menu，将其setEnable为false并注册到GSScrollView的MenuDelegate监听列表中
    void registerMenuRecursively(Node* node)
    {
        if(dynamic_cast<Menu*>(node))
        {
            registerMenuDelegate(dynamic_cast<Menu*>(node));
        }
        Vector<Node*>& array = node->getChildren();
        for(int i=0;i<array.size();i++)
        {
            Menu* menu = dynamic_cast<Menu*>(array.at(i));
            if(menu)
            {
                registerMenuDelegate(menu);
            }
            else
            {
                registerMenuRecursively(dynamic_cast<Node*>(array.at(i)));
            }
        }
    }
    
    // 递归检测node节点上的所有子menu，将其从GSScrollView的MenuDelegate监听列表中移除
    void unregisterMenuRecursively(Node* node)
    {
        Vector<Node*>& array = node->getChildren();
        for(int i=0;i<array.size();i++)
        {
            Menu* menu=dynamic_cast<Menu*>(array.at(i));
            if(menu)
            {
                unregisterMenuDelegate(menu);
            }
            else
            {
                unregisterMenuRecursively(dynamic_cast<Node*>(array.at(i)));
            }
        }
    };
    
    
    /** 注册监听的menu菜单，被添加的menu菜单将被设置touchEnabled为false */
    inline void registerMenuDelegate(Menu* menu)
    {
        if(!m_pMenuDelegateArray->containsObject(menu))
        {
            Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(menu);
            if(m_pMenuDelegateArray->count()==0)
            {
                m_pMenuDelegateArray->addObject(menu);
                return;
            }
            CCScrollMenu* scrooMenu=dynamic_cast<CCScrollMenu*>(menu);
            if(!scrooMenu)
            {
                m_pMenuDelegateArray->insertObject(menu, 0);
                return;
            }
            for(int i=0; i<m_pMenuDelegateArray->count()-1; i++)
            {
                CCScrollMenu* tempScrooMenu = dynamic_cast<CCScrollMenu*>(m_pMenuDelegateArray->getObjectAtIndex(i));
                if(!tempScrooMenu || tempScrooMenu->getHandlePriority() >= scrooMenu->getHandlePriority())
                {
                    continue;
                }
                m_pMenuDelegateArray->insertObject(menu,i);// Priority属性值越低，优先级越高，越放到数组的后面
                return;
            }
            m_pMenuDelegateArray->addObject(menu);// 最后添加到末尾
        }
    }
    
    /** 移除注册的menu菜单 */
    inline void unregisterMenuDelegate(Menu* menu)
    {
        if(m_pMenuDelegateArray->containsObject(menu))
        {
            m_pMenuDelegateArray->removeObject(menu);
        }
    }
    
    void setContainer(Node * pContainer);
    
    // optional
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    virtual void updateInsetAndPosition();
    virtual void updateInset();
    virtual void setDirection(ScrollView::Direction eDirection);
    void deaccelerateScrolling(float dt);
    
    void setDeaccelRate(float f)
    {
        CCAssert(f>0&&f<1, "value should between 0 and 1");
        m_fDeaccelRate=f;
    };
    inline void setBlockMoveMenu(bool flag){m_blockMoveMenu = flag;}
protected:
    /** 注册监听的menu菜单列表 */
    __Array* m_pMenuDelegateArray;
    /** 当前处理触摸事件的menu对象 */
    Menu* m_pCurrentMenu;
    // 边距的最大超出范围比例
    float m_fInsetRatio;
    // 移动标志(根据移动距离判定是否需要移动视口)
    bool m_bMove;
    static bool m_isMoved;
    float m_fDeaccelRate;
    bool m_nonstaticIsMoved; //非静态变量 和m_isMoved对应
    bool m_blockMoveMenu;
};

#endif /* defined(__FoxUGame__GSScrollView__) */
