//
//  FlipScrollView.h
//  EmpireLegion
//  整页滚动面板，一次翻动一页，每一页的显示节点和视口同等大小
//  Created by gaodan on 13-3-23.
//
//

#ifndef __EmpireLegion__FlipScrollView__
#define __EmpireLegion__FlipScrollView__

#include "GSScrollView.h"
#include "CCTableView.h"
#include "CCTableViewCell.h"

class FlipTableView;

/**
 * Sole purpose of this delegate is to single touch event in this version.
 */
class FlipTableViewDelegate : public ScrollViewDelegate
{
public:
    
    virtual void tableCellTouched(FlipTableView* table, TableViewCell* cell) = 0;

    virtual void tableCellHighlight(FlipTableView* table, TableViewCell* cell){};

    virtual void tableCellUnhighlight(FlipTableView* table, TableViewCell* cell){};

    virtual void tableCellWillRecycle(FlipTableView* table, TableViewCell* cell){};
    
};
class FlipTableViewDataSource
{
public:

    virtual ~FlipTableViewDataSource() {}

    virtual Size tableCellSizeForIndex(FlipTableView *table, ssize_t idx) {
        return cellSizeForTable(table);
    };

    virtual Size cellSizeForTable(FlipTableView *table) {
        return Size::ZERO;
    };

    virtual TableViewCell* tableCellAtIndex(FlipTableView *table, ssize_t idx) = 0;

    virtual ssize_t numberOfCellsInTableView(FlipTableView *table) = 0;
    // 设置cell 的menu为不可用
    virtual void unableMenu(TableViewCell *)=0;
    // 切换回调
    virtual void changeCell()=0;
};

// 目前基本把TableView的结构照搬过来，再进行适当修改
class FlipTableView : public GSScrollView, public ScrollViewDelegate
{
public:
    enum
    {
        SLIDE_SPEED=3,// 滑动节点的速度阀值
    };
    
    enum class VerticalFillOrder
    {
        TOP_DOWN,
        BOTTOM_UP
    };
    
    static FlipTableView* create();
    
    static FlipTableView* create(FlipTableViewDataSource* dataSource, Size size);
    
    static FlipTableView* create(FlipTableViewDataSource* dataSource, Size size, Node *container);
    
    FlipTableView();
    
    virtual ~FlipTableView();
    
    bool initWithViewSize(Size size, Node* container = NULL);
    FlipTableViewDataSource* getDataSource() { return _dataSource; }
    void setDataSource(FlipTableViewDataSource* source) { _dataSource = source; }
    FlipTableViewDelegate* getDelegate() { return _tableViewDelegate; }
    void setDelegate(FlipTableViewDelegate* pDelegate) { _tableViewDelegate = pDelegate; }
    
    void setVerticalFillOrder(VerticalFillOrder order);
    VerticalFillOrder getVerticalFillOrder();
    
    void updateCellAtIndex(ssize_t idx);
    void insertCellAtIndex(ssize_t idx);
    void removeCellAtIndex(ssize_t idx);
    void reloadData();
    TableViewCell *dequeueCell();
    
    /**
     * Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
     *
     * @param idx index
     * @return a cell at a given index
     */
    TableViewCell *cellAtIndex(ssize_t idx);
    
    // Overrides
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view)  override {}
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent) override;
    // 设置m_touchListener是否吞噬触摸事件
    void setSwallowTouches(bool isSwallow);
    // 根据index设置偏移量  Direction::VERTICAL未测试
    void setOffsetByIndex(int index);
    // 当前显示cell index  只适用cellSize相同且重写cellSizeForTable
    ssize_t getCurrIndex();
protected:
    
    long __indexFromOffset(Vec2 offset);
    long _indexFromOffset(Vec2 offset);
    Vec2 __offsetFromIndex(ssize_t index);
    Vec2 _offsetFromIndex(ssize_t index);
    
    void _moveCellOutOfSight(TableViewCell *cell);
    void _setIndexForCell(ssize_t index, TableViewCell *cell);
    void _addCellIfNecessary(TableViewCell * cell);
    
    void _updateCellPositions();

    // touchEnd 动画后回调
    void _rollCallBack(Ref* ps);

    TableViewCell *_touchedCell;
    /**
     * vertical direction of cell filling
     */
    VerticalFillOrder _vordering;
    
    /**
     * index set to query the indexes of the cells used.
     */
    std::set<ssize_t>* _indices;
    
    /**
     * vector with all cell positions
     */
    std::vector<float> _vCellsPositions;
    //NSMutableIndexSet *indices_;
    /**
     * cells that are currently in the table
     */
    Vector<TableViewCell*> _cellsUsed;
    /**
     * free list of cells
     */
    Vector<TableViewCell*> _cellsFreed;
    /**
     * weak link to the data source object
     */
    FlipTableViewDataSource* _dataSource;
    /**
     * weak link to the delegate object
     */
    FlipTableViewDelegate* _tableViewDelegate;
    
    Direction _oldDirection;
    
    bool _isUsedCellsDirty;
    
public:
    void _updateContentSize();
    
};

#endif
