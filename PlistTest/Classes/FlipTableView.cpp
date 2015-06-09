//
//  FlipScrollView.cpp
//  EmpireLegion
//  整页滚动面板，一次翻动一页，每一页的显示节点和视口同等大小
//  Created by gaodan on 13-3-23.
//
//

#include "FlipTableView.h"

FlipTableView* FlipTableView::create()
{
    return FlipTableView::create(nullptr, Size::ZERO);
}

FlipTableView* FlipTableView::create(FlipTableViewDataSource* dataSource, Size size)
{
    return FlipTableView::create(dataSource, size, NULL);
}

FlipTableView* FlipTableView::create(FlipTableViewDataSource* dataSource, Size size, Node *container)
{
    FlipTableView* table = new FlipTableView();
    table->initWithViewSize(size, container);
    table->setInsetRatio(0);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool FlipTableView::initWithViewSize(Size size, Node* container/* = NULL*/)
{
    if (GSScrollView::initWithViewSize(size,container))
    {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        _vordering      = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);
        
        GSScrollView::setDelegate(this);
        return true;
    }
    return false;
}

FlipTableView::FlipTableView()
: _touchedCell(nullptr)
, _indices(nullptr)
, _dataSource(nullptr)
, _tableViewDelegate(nullptr)
, _oldDirection(Direction::NONE)
, _isUsedCellsDirty(false)
{
    
}

FlipTableView::~FlipTableView()
{
    CC_SAFE_DELETE(_indices);
}

void FlipTableView::setVerticalFillOrder(VerticalFillOrder fillOrder)
{
    if (_vordering != fillOrder)
    {
        _vordering = fillOrder;
        if (!_cellsUsed.empty())
        {
            this->reloadData();
        }
    }
}

FlipTableView::VerticalFillOrder FlipTableView::getVerticalFillOrder()
{
    return _vordering;
}

void FlipTableView::reloadData()
{
    _oldDirection = Direction::NONE;
    
    for(const auto &cell : _cellsUsed) {
        if(_tableViewDelegate != nullptr) {
            _tableViewDelegate->tableCellWillRecycle(this, cell);
        }
        
        _cellsFreed.pushBack(cell);
        
        cell->reset();
        if (cell->getParent() == this->getContainer())
        {
            this->getContainer()->removeChild(cell, true);
        }
    }
    
    _indices->clear();
    _cellsUsed.clear();
    
    this->_updateCellPositions();
    this->_updateContentSize();
    if (_dataSource->numberOfCellsInTableView(this) > 0)
    {
        this->scrollViewDidScroll(this);
    }
}

TableViewCell *FlipTableView::cellAtIndex(ssize_t idx)
{
    if (_indices->find(idx) != _indices->end())
    {
        for (const auto& cell : _cellsUsed)
        {
            if (cell->getIdx() == idx)
            {
                return cell;
            }
        }
    }
    
    return nullptr;
}

void FlipTableView::updateCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    TableViewCell* cell = this->cellAtIndex(idx);
    if (cell)
    {
        this->_moveCellOutOfSight(cell);
    }
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
}

void FlipTableView::insertCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems || idx > countOfItems-1)
    {
        return;
    }
    
    long newIdx = 0;
    
    auto cell = cellAtIndex(idx);
    if (cell)
    {
        newIdx = _cellsUsed.getIndex(cell);
        // Move all cells behind the inserted position
        for (long i = newIdx; i < _cellsUsed.size(); i++)
        {
            cell = _cellsUsed.at(i);
            this->_setIndexForCell(cell->getIdx()+1, cell);
        }
    }
    
    //insert a new cell
    cell = _dataSource->tableCellAtIndex(this, idx);
    this->_setIndexForCell(idx, cell);
    this->_addCellIfNecessary(cell);
    
    this->_updateCellPositions();
    this->_updateContentSize();
}

void FlipTableView::removeCellAtIndex(ssize_t idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    long uCountOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    ssize_t newIdx = 0;
    
    TableViewCell* cell = this->cellAtIndex(idx);
    if (!cell)
    {
        return;
    }
    
    newIdx = _cellsUsed.getIndex(cell);
    
    //remove first
    this->_moveCellOutOfSight(cell);
    
    _indices->erase(idx);
    this->_updateCellPositions();
    
    for (ssize_t i = _cellsUsed.size()-1; i > newIdx; i--)
    {
        cell = _cellsUsed.at(i);
        this->_setIndexForCell(cell->getIdx()-1, cell);
    }
}

TableViewCell *FlipTableView::dequeueCell()
{
    TableViewCell *cell;
    
    if (_cellsFreed.empty()) {
        cell = nullptr;
    } else {
        cell = _cellsFreed.at(0);
        cell->retain();
        _cellsFreed.erase(0);
        cell->autorelease();
    }
    return cell;
}

void FlipTableView::_addCellIfNecessary(TableViewCell * cell)
{
    if (cell->getParent() != this->getContainer())
    {
        this->getContainer()->addChild(cell);
    }
    _cellsUsed.pushBack(cell);
    _indices->insert(cell->getIdx());
    _isUsedCellsDirty = true;
}

void FlipTableView::_updateContentSize()
{
    Size size = Size::ZERO;
    ssize_t cellsCount = _dataSource->numberOfCellsInTableView(this);
    
    if (cellsCount > 0 && cellsCount < _vCellsPositions.size())
    {
        float maxPosition = _vCellsPositions[cellsCount];
        
        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
                size = Size(maxPosition, _viewSize.height);
                break;
            default:
                size = Size(_viewSize.width, maxPosition);
                break;
        }
    }
    
    this->setContentSize(size);
    
	if (_oldDirection != _direction)
	{
		if (_direction == Direction::HORIZONTAL)
		{
			this->setContentOffset(Vec2(0,0));
		}
		else
		{
			this->setContentOffset(Vec2(0,this->minContainerOffset().y));
		}
		_oldDirection = _direction;
	}
    
}

Vec2 FlipTableView::_offsetFromIndex(ssize_t index)
{
    Vec2 offset = this->__offsetFromIndex(index);
    
    const Size cellSize = _dataSource->tableCellSizeForIndex(this, index);
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    }
    return offset;
}

Vec2 FlipTableView::__offsetFromIndex(ssize_t index)
{
    Vec2 offset;
    Size  cellSize;
    
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            offset = Vec2(_vCellsPositions[index], 0.0f);
            break;
        default:
            offset = Vec2(0.0f, _vCellsPositions[index]);
            break;
    }
    return offset;
}

long FlipTableView::_indexFromOffset(Vec2 offset)
{
    long index = 0;
    const long maxIdx = _dataSource->numberOfCellsInTableView(this) - 1;
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y;
    }
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }
    return index;
}

long FlipTableView::__indexFromOffset(Vec2 offset)
{
    long low = 0;
    long high = _dataSource->numberOfCellsInTableView(this) - 1;
    float search;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            search = offset.x;
            break;
        default:
            search = offset.y;
            break;
    }
    
    while (high >= low)
    {
        long index = low + (high - low) / 2;
        float cellStart = _vCellsPositions[index];
        float cellEnd = _vCellsPositions[index + 1];
        
        if (search >= cellStart && search <= cellEnd)
        {
            return index;
        }
        else if (search < cellStart)
        {
            high = index - 1;
        }
        else
        {
            low = index + 1;
        }
    }
    
    if (low <= 0) {
        return 0;
    }
    return -1;
}

void FlipTableView::_moveCellOutOfSight(TableViewCell *cell)
{
    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->tableCellWillRecycle(this, cell);
    }
    
    _cellsFreed.pushBack(cell);
    _cellsUsed.eraseObject(cell);
    _isUsedCellsDirty = true;
    
    _indices->erase(cell->getIdx());
    cell->reset();
    
    if (cell->getParent() == this->getContainer())
    {
        unregisterMenuRecursively(cell);
        this->getContainer()->removeChild(cell, true);;
    }
}

void FlipTableView::_setIndexForCell(ssize_t index, TableViewCell *cell)
{
    cell->setAnchorPoint(Vec2(0.0f, 0.0f));
    cell->setPosition(this->_offsetFromIndex(index));
    cell->setIdx(index);
}
void FlipTableView::_updateCellPositions()
{
    long cellsCount = _dataSource->numberOfCellsInTableView(this);
    _vCellsPositions.resize(cellsCount + 1, 0.0);
    
    if (cellsCount > 0)
    {
        float currentPos = 0;
        Size cellSize;
        for (int i=0; i < cellsCount; i++)
        {
            _vCellsPositions[i] = currentPos;
            cellSize = _dataSource->tableCellSizeForIndex(this, i);
            switch (this->getDirection())
            {
                case Direction::HORIZONTAL:
                    currentPos += cellSize.width;
                    break;
                default:
                    currentPos += cellSize.height;
                    break;
            }
        }
        _vCellsPositions[cellsCount] = currentPos;//1 extra value allows us to get right/bottom of the last cell
    }
}
void FlipTableView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems)
    {
        return;
    }
    
    if (_isUsedCellsDirty)
    {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](TableViewCell *a, TableViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->scrollViewDidScroll(this);
    }
    
    ssize_t startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = offset.y + _viewSize.height/this->getContainer()->getScaleY();
    }
    startIdx = this->_indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX)
	{
		startIdx = countOfItems - 1;
	}
    
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y -= _viewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += _viewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += _viewSize.width/this->getContainer()->getScaleX();
    
    endIdx   = this->_indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = countOfItems - 1;
	}
    
#if 0 // For Testing.
    Ref* pObj;
    int i = 0;
    CCARRAY_FOREACH(_cellsUsed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(_cellsFreed, pObj)
    {
        TableViewCell* pCell = static_cast<TableViewCell*>(pObj);
        log("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.at(0);
        idx = cell->getIdx();
        
        while(idx < startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.at(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (!_cellsUsed.empty())
    {
        auto cell = _cellsUsed.back();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (!_cellsUsed.empty())
            {
                cell = _cellsUsed.back();
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    
    for (long i = startIdx; i <= endIdx; i++)
    {
        if (_indices->find(i) != _indices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
}
bool FlipTableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    bool touchResult = GSScrollView::onTouchBegan(pTouch, pEvent);
    
    if(_touches.size() == 1)
    {
        long index;
        Vec2 point;
        
        point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        
        index = this->_indexFromOffset(point);
		if (index == CC_INVALID_INDEX)
		{
			_touchedCell = nullptr;
		}
        else
		{
			_touchedCell  = this->cellAtIndex(index);
		}
        
        if (_touchedCell && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellHighlight(this, _touchedCell);
        }
    }
    else if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
    
    if(_dragging)// 停止自动滚动的action
    {
        this->getContainer()->stopAllActions();
    }
    
    return touchResult;
}

void FlipTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    // 以下代码将GSScrollView与ScrollView的onTouchEnded里的代码进行整合,所以不需要调用父类的onTouchEnded方法
    if (!this->isVisible())
    {
        return;
    }
    if (_touchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);
        
		if (bb.containsPoint(pTouch->getLocation()) && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
            _tableViewDelegate->tableCellTouched(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
    
    if (std::find(_touches.begin(), _touches.end(), pTouch) != _touches.end())
    {
        if (_touches.size() == 1 && _touchMoved)
        {
//            this->schedule(schedule_selector(CCScrollView::deaccelerateScrolling));
            Size cellSize  = _dataSource->cellSizeForTable(this);
            int pageNum=0;
            float diss=0;
            Vec2 maxOffset=_maxInset;
            Vec2 minOffset=_minInset;
            Vec2 offsetPoint=this->getContentOffset();
            pageNum=offsetPoint.x/cellSize.width;
            
            // 注意判定table滚动的方向是水平还是垂直
            if(_direction==ScrollView::Direction::HORIZONTAL)// 水平方向
            {
//                CCLOG("m_tScrollDistance.x=%f",m_tScrollDistance.x);
                diss=fabs(offsetPoint.x-pageNum*cellSize.width);// 水平方向diss的取值只能是小于等于0
                // 计算当前的容器的水平方向的移动距离
                if(_scrollDistance.x>0)// 向右滚动
                {
                    if(_scrollDistance.x>SLIDE_SPEED)// 向右侧一页滚动
                    {
                        if(offsetPoint.x>=maxOffset.x)// 右侧边界判定
                        {
                            diss=0;
                        }
//                        else diss=diss;
                    }
                    else// 滚动速度不够的情况下判定节点中心在视口中点的位置，从而决定向左或者向右翻页，还要考虑最左和最右侧的边界情况
                    {
                        if(diss<(cellSize.width/2))// 未过半，向右侧一页滚动
                        {
                            if(offsetPoint.x>=maxOffset.x)// 右侧边界判定
                            {
                                diss=0;
                            }
//                            else diss=diss;
                        }
                        else// 回到左侧一页
                        {
                            if(offsetPoint.x<minOffset.x)// 左侧边界判定
                            {
                                diss=0;
                            }
                            else
                            {
                                diss=diss-cellSize.width;
                            }
                        }
                    }
                }
                else if(_scrollDistance.x<0)// 向左滚动
                {
                    if(_scrollDistance.x<-SLIDE_SPEED)// 向左侧一页滚动
                    {
                        if(offsetPoint.x<=minOffset.x)// 左侧边界判定
                        {
                            diss=0;
                        }
                        else
                        {
                            diss=diss-cellSize.width;
                        }
                    }
                    else// 滚动速度不够的情况下判定节点中心在视口中点的位置，从而决定向左或者向右翻页，还要考虑最左和最右侧的边界情况
                    {
                        if(diss<(cellSize.width/2))// 向右侧一页滚动
                        {
                            if(offsetPoint.x>=maxOffset.x)// 右侧边界判定
                            {
                                diss=0;
                            }
//                            else diss=diss;
                        }
                        else// 回到左侧一页
                        {
                            if(offsetPoint.x<minOffset.x)// 左侧边界判定
                            {
                                diss=0;
                            }
                            else
                            {
                                diss=diss-cellSize.width;
                            }
                        }
                    }
                }
                if(diss!=0)
                {
                    Vec2 point=Vec2(diss,0);
                    EaseSineOut* easeSineOut=EaseSineOut::create(MoveBy::create(0.2, point));
                    Sequence* sequence = Sequence::create(easeSineOut,
                                                CallFuncN::create(CC_CALLBACK_1(FlipTableView::_rollCallBack, this)),
                                                NULL);
                    this->getContainer()->runAction(sequence);
                }
                else //边缘未做拖动也要回调
                {
                    _rollCallBack(NULL);
                }
            }
            else// 垂直方向
            {
                
            }
            // 根据在cctouchMoved中m_tScrollDistance的计算值，判断移动的方向和速度，根据方向和速度判断容器的滚动方向
            
        }
//        _touche.erase(pTouch);
        auto touchIter = std::find(_touches.begin(), _touches.end(), pTouch);
        if (touchIter != _touches.end())
        {
            _touches.erase(touchIter);
        }
    }
    
    if (_touches.size() == 0)
    {
        _dragging = false;
        _touchMoved = false;
        m_bMove=false;
    }
    
    if(m_pCurrentMenu)// 添加的代码
    {
        m_pCurrentMenu->onTouchEnded(pTouch, pEvent);
        m_pCurrentMenu=NULL;
    }
}

void FlipTableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    GSScrollView::onTouchMoved(pTouch, pEvent);
    
    if (_touchedCell && isTouchMoved())
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
}

void FlipTableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    GSScrollView::onTouchCancelled(pTouch, pEvent);
    
    if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }
        
        _touchedCell = nullptr;
    }
}

// 设置m_touchListener是否吞噬触摸事件
void FlipTableView::setSwallowTouches(bool isSwallow)
{
    if(_touchListener)
    {
        _touchListener->setSwallowTouches(isSwallow);
    }
}

// 根据index设置偏移量  Direction::VERTICAL未测试
void  FlipTableView::setOffsetByIndex(int  index)
{
    Vec2 vec = _offsetFromIndex(index);
    if(_direction == Direction::HORIZONTAL)
        this->setContentOffset(Vec2(-vec.x,0));
    else
        this->setContentOffset(Vec2(0,-vec.y));
}

// 当前显示cell index  只适用cellSize相同且重写cellSizeForTable
ssize_t FlipTableView::getCurrIndex()
{
    if(_direction == Direction::HORIZONTAL)
    {
         return fabs(this->getContentOffset().x) / _dataSource->cellSizeForTable(this).width;
    }
    return 0;
}

// touchEnd 动画后回调
void FlipTableView::_rollCallBack(Ref* pe)
{
    _dataSource->changeCell();
}