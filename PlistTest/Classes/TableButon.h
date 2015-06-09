//
//  TableButon.h
//  PlistTest
//
//  Created by XD on 15/5/7.
//
//

#ifndef __PlistTest__TableButon__
#define __PlistTest__TableButon__

#include "cocos2d.h"
#include "CCTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TableButon : public Layer,
public TableViewDelegate,
public TableViewDataSource
{
public:
    TableButon();
    virtual ~TableButon();
    bool init();
    CREATE_FUNC(TableButon);
    static TableButon* createWithTable(__Array* arr);
    void initWithTable(__Array* arr);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) {
        return cellSizeForTable(table);
    };
    
    virtual Size cellSizeForTable(TableView *table) ;
    
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
   
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    //循环
    void tableCellWillRecycle(TableView* table, TableViewCell* cell);
private:
    
    
};


#endif /* defined(__PlistTest__TableButon__) */
