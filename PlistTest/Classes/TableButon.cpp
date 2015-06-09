//
//  TableButon.cpp
//  PlistTest
//
//  Created by XD on 15/5/7.
//
//

#include "TableButon.h"



TableButon::TableButon()
{

}

TableButon::~TableButon()
{

}

TableButon* TableButon::createWithTable(__Array* arr)
{
    TableButon* buton = TableButon::create();
    buton->initWithTable(arr);
    return buton;
}

void TableButon::initWithTable(__Array* arr)
{
    TableView* view = TableView::create(this,Size(320,250));
    view->setDirection(ScrollView::Direction::HORIZONTAL);
    view->setDelegate(this);
    view->setDataSource(this);
    this->addChild(view);
}

bool TableButon::init()
{
    return Layer::init();
}

void TableButon::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void TableButon::tableCellHighlight(TableView* table, TableViewCell* cell)
{
//    Sprite* sprite =Sprite::create("res/btn_green_1_d.png");
//    sprite->setAnchorPoint(Vec2::ZERO);
    dynamic_cast<Sprite*>(cell->getChildren().at(0))->setColor(Color3B(128,120,0));
}


TableViewCell* TableButon::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    log("index = %zd",idx );
    char buf[200];
    sprintf(buf, "res/alliance_island_%ld.png",idx+1);
    Sprite* sprite = Sprite::create(buf);
    sprite->setAnchorPoint(Vec2::ZERO);
    if(!cell)
    {
        cell = TableViewCell::create();
        cell->setIdx(idx);
        cell->addChild(sprite);
    }
    return cell;
}

ssize_t TableButon::numberOfCellsInTableView(TableView *table)
{
    return 6;
}


Size TableButon::cellSizeForTable(TableView *table)
{
    return Size(320,240);
}


void TableButon::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{

}