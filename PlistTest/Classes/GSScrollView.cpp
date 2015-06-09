//
//  GSScrollView.cpp
//  FoxUGame
//  滚动视口
//  Created by gaodan on 12-10-24.
//
//

#include "GSScrollView.h"

int GSScrollView::moveOffsetValue=MOVE_OFFSET_VALUE;
bool GSScrollView::m_isMoved=true;
void GSScrollView::setIsMoved(bool flag)
{
    m_isMoved=flag;
}

GSScrollView::GSScrollView(void)
: m_fInsetRatio(0.2f)
, m_bMove(false)
, m_fDeaccelRate(0.95f)
, m_nonstaticIsMoved(true)
, m_blockMoveMenu(false)
{
    m_pMenuDelegateArray=new __Array();
    m_pCurrentMenu = NULL;
}

GSScrollView::~GSScrollView(void)
{
    CC_SAFE_RELEASE(m_pMenuDelegateArray);
}

/**
 * Returns an autoreleased scroll view object.
 *
 * @param size view size
 * @param container parent object
 * @return autoreleased scroll view object
 */
GSScrollView* GSScrollView::create(Size size, Node* container)
{
    GSScrollView* pRet = new GSScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

/**
 * Returns an autoreleased scroll view object.
 *
 * @param size view size
 * @param container parent object
 * @return autoreleased scroll view object
 */
GSScrollView* GSScrollView::create()
{
    GSScrollView* pRet = new GSScrollView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void GSScrollView::setContainer(Node * pContainer)
{
    this->removeAllChildrenWithCleanup(true);
    
    if (!pContainer) return;
    
    this->_container = pContainer;
    
    this->_container->ignoreAnchorPointForPosition(false);
    this->_container->setAnchorPoint(Vec2(0.0f, 0.0f));
    
    this->addChild(this->_container);
    this->setContentSize(pContainer->getContentSize());
    this->setViewSize(this->_viewSize);
}

bool GSScrollView::onTouchBegan(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    for (Node *c = this->_parent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    Rect frame;
    Point frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = Rect(frameOriginal.x, frameOriginal.y, _viewSize.width, _viewSize.height);
    
    if (_touchMoved||
        !frame.containsPoint(_container->convertToWorldSpace(_container->convertTouchToNodeSpace(touch))))
    {
        return false;
    }
    
    if (_touches.size()<2 && std::find(_touches.begin(), _touches.end(), touch) == _touches.end())
    {
        _touches.push_back(touch);
    }
    
    if (_touches.size() == 1)
    { // scrolling
        _touchPoint         = this->convertTouchToNodeSpace(touch);
        _touchMoved         = false;
        _dragging           = true; //dragging started
        _scrollDistance     = Vec2(0.0f, 0.0f);
        _touchLength        = 0.0f;
    }
    else if (_touches.size() == 2)
    {
        _touchPoint  = (this->convertTouchToNodeSpace((Touch*)_touches.at(0)).getMidpoint(
                                     this->convertTouchToNodeSpace((Touch*)_touches.at(1))) );
        _touchLength = (_container->convertTouchToNodeSpace((Touch*)_touches.at(0)).getDistance(
                                     _container->convertTouchToNodeSpace((Touch*)_touches.at(1))) );
        _dragging  = false;
    }

    if(!m_pCurrentMenu)
    {
        for(int i=m_pMenuDelegateArray->count()-1;i>=0;i--)// 添加的代码
        {
            Menu* menu=dynamic_cast<Menu*>(m_pMenuDelegateArray->getObjectAtIndex(i));
            if(menu)
            {
                if(menu->onTouchBegan(touch, event))
                {
                    m_pCurrentMenu=menu;
                    break;
                }
            }
        }
    }
    return true;
}

void GSScrollView::onTouchMoved(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    if(m_pCurrentMenu)// 添加的代码
    {
        if(m_blockMoveMenu)
        {
            Touch* t = new Touch();
            t->setTouchInfo(touch->getID(), 1000000, 1000000);
            m_pCurrentMenu->onTouchMoved(t, event);
            CC_SAFE_DELETE(t);
        }
        else
            m_pCurrentMenu->onTouchMoved(touch, event);
    }
    if(!m_isMoved)
    {
        return;
    }
    if(!m_nonstaticIsMoved)
        return;
    if (std::find(_touches.begin(), _touches.end(), touch) != _touches.end())
    {
        if (_touches.size() == 1 && _dragging)
        { // scrolling
            Point moveDistance, newPoint, maxInset, minInset;
            Rect  frame;
            float newX, newY;
            
            Point frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
            frame = Rect(frameOriginal.x, frameOriginal.y, _viewSize.width, _viewSize.height);
            
            newPoint     = this->convertTouchToNodeSpace((Touch*)_touches.at(0));
            moveDistance = newPoint - _touchPoint;
            if(!m_bMove&&fabs(moveDistance.x)<moveOffsetValue&&fabs(moveDistance.y)<moveOffsetValue)
            {
                return;
            }
            if(!m_bMove)
            {
                _touchMoved = true;
                m_bMove=true;
                _touchPoint  = newPoint;
                return;
            }
            
            _touchPoint  = newPoint;
            
            if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
            {
                switch (_direction)
                {
                    case ScrollView::Direction::VERTICAL:
                        moveDistance = Vec2(0.0f, moveDistance.y);
                        break;
                    case ScrollView::Direction::HORIZONTAL:
                        moveDistance = Vec2(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }
                
                _container->setPosition(_container->getPosition() + moveDistance);
                
                maxInset = _maxInset;
                minInset = _minInset;
                
                //check to see if offset lies within the inset bounds
                newX     = MIN(_container->getPosition().x, maxInset.x);
                newX     = MAX(newX, minInset.x);
                newY     = MIN(_container->getPosition().y, maxInset.y);
                newY     = MAX(newY, minInset.y);
                
                _scrollDistance     = moveDistance - Vec2(newX - _container->getPosition().x, newY - _container->getPosition().y);
                this->setContentOffset(Vec2(newX, newY));
            }
        }
        else if (_touches.size() == 2 && !_dragging)
        {
            const float len = (_container->convertTouchToNodeSpace((Touch*)_touches.at(0)).getDistance(
                                          _container->convertTouchToNodeSpace((Touch*)_touches.at(1))) );
            
            float scale=this->getZoomScale()*len/_touchLength;
            scale=MAX(_minScale, MIN(_maxScale, scale));// 使缩放比例不超过设定的缩放最大最小范围
            bool flag=_container->getScale()!=scale;
            this->setZoomScale(scale);
            if(flag)
            {
                updateInset();// 如果缩放比例不一样，则更新显示范围
                
                // 判断container的offset偏移，重设container的位置，使其始终显示在屏幕范围内
                Point maxInset = _maxInset;
                Point minInset = _minInset;
                
                //check to see if offset lies within the inset bounds
                float newX = MIN(_container->getPosition().x, maxInset.x);
                newX = MAX(newX, minInset.x);
                float newY = MIN(_container->getPosition().y, maxInset.y);
                newY = MAX(newY, minInset.y);
                this->setContentOffset(Vec2(newX, newY));
            }
            if(scale==_minScale||scale==_maxScale)
            {
                _touchLength=len;
            }
        }
    }
}

void GSScrollView::onTouchEnded(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
//    ScrollView::onTouchEnded(touch,event);
    
    // 从引擎scrollView中挪过来的代码 begin
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    
    if (touchIter != _touches.end())
    {
        if (_touches.size() == 1 && _touchMoved)
        {
            this->schedule(schedule_selector(GSScrollView::deaccelerateScrolling));
        }
        _touches.erase(touchIter);
    } 
    
    if (_touches.size() == 0)
    {
        _dragging = false;    
        _touchMoved = false;
    }
    // 从引擎scrollView中挪过来的代码 end
    
    if(_scrollDistance.x==0&&_scrollDistance.y==0&&_touchLength==0)
    {
        relocateContainer(true);
    }
    if (_touches.size() == 0)
    {
        m_bMove=false;
    }
    if(m_pCurrentMenu)// 添加的代码
    {
        m_pCurrentMenu->onTouchEnded(touch, event);
        m_pCurrentMenu=NULL;
    }
}

void GSScrollView::onTouchCancelled(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
    
    
    _touches.clear();
//    ScrollView::onTouchCancelled(touch, event);
    
    if(m_pCurrentMenu)// 添加的代码
    {
        m_pCurrentMenu->onTouchCancelled(touch, event);
        m_pCurrentMenu=NULL;
    }
}

void GSScrollView::updateInsetAndPosition()
{
    updateInset();
    
    Point maxInset = _maxInset;
    Point minInset = _minInset;
    
    //check to see if offset lies within the inset bounds
    float newX = MIN(_container->getPosition().x, maxInset.x);
    newX = MAX(newX, minInset.x);
    float newY = MIN(_container->getPosition().y, maxInset.y);
    newY = MAX(newY, minInset.y);
    this->setContentOffset(Vec2(newX, newY));

}

void GSScrollView::updateInset()
{
    if (this->getContainer() != NULL)
	{
		_maxInset = this->maxContainerOffset();
        float offsetX=0;
        float offsetY=0;
        if(_direction == ScrollView::Direction::HORIZONTAL || _direction == ScrollView::Direction::BOTH)
        {
            offsetX=_viewSize.width * m_fInsetRatio;
        }
        if(_direction==ScrollView::Direction::VERTICAL||_direction==ScrollView::Direction::BOTH)
        {
            offsetY=_viewSize.height * m_fInsetRatio;
        }
		_maxInset = Vec2(_maxInset.x + offsetX,
                          _maxInset.y + offsetY);
        
		_minInset = this->minContainerOffset();
		_minInset = Vec2(_minInset.x - offsetX,
                          _minInset.y - offsetY);
	}
}

void GSScrollView::setDirection(ScrollView::Direction eDirection)
{
    ScrollView::setDirection(eDirection);
    updateInset();
}
using namespace cocos2d::extension;
void GSScrollView::deaccelerateScrolling(float dt)
{
    if (_dragging)
    {
        this->unschedule(schedule_selector(GSScrollView::deaccelerateScrolling));
        _scrollDistance.x=0;
        _scrollDistance.y=0;
        return;
    }
    
    float newX, newY;
    Vec2 maxInset, minInset;
    
    _container->setPosition(_container->getPosition() + _scrollDistance);
    
    if (_bounceable)
    {
        maxInset = _maxInset;
        minInset = _minInset;
    }
    else
    {
        maxInset = this->maxContainerOffset();
        minInset = this->minContainerOffset();
    }
    
//    newX = _container->getPosition().x;
//    newY = _container->getPosition().y;
    newX     = MIN(_container->getPosition().x, maxInset.x);
    newX     = MAX(newX, minInset.x);
    newY     = MIN(_container->getPosition().y, maxInset.y);
    newY     = MAX(newY, minInset.y);
    
//    _scrollDistance     = _scrollDistance * 0.95f;
    _scrollDistance     = _scrollDistance * m_fDeaccelRate;
    this->setContentOffset(Vec2(newX,newY));
    
//    if ((fabsf(_scrollDistance.x) <= 1.0f &&
//         fabsf(_scrollDistance.y) <= 1.0f) ||
//        newY >= maxInset.y || newY <= minInset.y ||
//        newX >= maxInset.x || newX <= minInset.x)
//    {
//        this->unschedule(schedule_selector(GSScrollView::deaccelerateScrolling));
//        this->relocateContainer(true);
//    }
    if ((fabsf(_scrollDistance.x) <= 1.0f &&
         fabsf(_scrollDistance.y) <= 1.0f) ||(newX == maxInset.x&&newY == maxInset.y)||(newX == maxInset.x&&newY == minInset.y)||(newX == minInset.x&&newY == maxInset.y)||(newX == minInset.x&&newY == minInset.y))
    {
        this->unschedule(schedule_selector(GSScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
        _scrollDistance.x=0;
        _scrollDistance.y=0;
    }
}



