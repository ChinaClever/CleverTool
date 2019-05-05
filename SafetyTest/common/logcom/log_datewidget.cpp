/*
 * log_datewidget.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "log_datewidget.h"
#include "ui_log_datewidget.h"
#include "common.h"
#include "log_exportdlg.h"

LOG_DateWidget::LOG_DateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LOG_DateWidget)
{
    ui->setupUi(this);

    mDateHash = new LOG_DateHash();
    //mTreeWidget->setSortingEnabled(true);
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this,SLOT(selectedItemSlot(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSLot(QModelIndex)));

    QStringList headers;
    headers << tr("日期列表");
     ui->treeWidget->setHeaderLabels(headers);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

LOG_DateWidget::~LOG_DateWidget()
{
    delete ui;
    delete mDateHash;
}

void LOG_DateWidget::setTable(const QString &name)
{
    mTableName = name;
    mDateHash->updateTab(name);
    updateWidget();
}




void LOG_DateWidget::initWidget(void)
{
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1); //设置列数
}


int LOG_DateWidget::dayShow(QString &month, QTreeWidgetItem *item)
{
    QStringList dayList;
    mDateHash->getDayList(month, dayList);

    for(int j=0; j<dayList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, dayList[j]);
        item->addChild(subItem);
    }

    return dayList.size();
}

int LOG_DateWidget::monthShow(QString &year, QTreeWidgetItem *item)
{
    QStringList monthList;
    mDateHash->getMonthList(year, monthList);
    for(int j=0; j<monthList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, monthList[j]);

        dayShow(monthList[j],subItem);
        item->addChild(subItem);
    }

    return monthList.size();
}


void LOG_DateWidget::yearShow(void)
{
    QStringList yearList;
    mDateHash->getYearList(yearList);

    initWidget();
    QList<QTreeWidgetItem *> rootList;
    for(int d=0; d<yearList.size(); ++d)
    {
        QString year = yearList.at(d);
        QTreeWidgetItem *yearItem = new QTreeWidgetItem(ui->treeWidget);
        yearItem->setText(0, year+ QObject::tr("年"));
        rootList.append(yearItem);

        int rtn = monthShow(year, yearItem);
        if(rtn == 0) {
            rootList.removeLast();
            delete yearItem;
        }
    }

    ui->treeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
}

void LOG_DateWidget::updateWidget()
{
    yearShow();
}

void LOG_DateWidget::updateSlot()
{
    mDateHash->updateTab(mTableName);
    updateWidget();
}

bool LOG_DateWidget::isDate(const QString &date)
{
    bool ret = false;
    QStringList list = date.split("-",QString::SkipEmptyParts);
    if(list.size() == 3) {
        ret = true;
    }
    return ret;
}

bool LOG_DateWidget::selectedItemSlot(QTreeWidgetItem *item,int column)
{
    if(item->parent())
    {
        QString str = item->text(column);
        bool ret = isDate(str);
        if(ret)  {
            str = tr("date like '%%1%'").arg(str);
            emit selectedSig(str);
        }
    }

    return true;
}

/**
 * @brief 查找定位
 */
void LOG_DateWidget::seekDate(const QString &date)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it)
    {
        if((*it)->text(0) == date)
        {
            ui->treeWidget->collapseAll(); //全部关闭
            ui->treeWidget->setCurrentIndex(ui->treeWidget->rootIndex());
            ui->treeWidget->setCurrentItem((*it));

            QModelIndex index = ui->treeWidget->currentIndex();
            ui->treeWidget->expand(index);
        }
        ++it;
    }
}


/**
 * @brief 实现单击点开操作
 * @param index
 */
void LOG_DateWidget::clickedSLot(QModelIndex index)
{
    bool ret = ui->treeWidget->isExpanded(index);
    if(ret)
        ui->treeWidget->collapse(index);
    else
        ui->treeWidget->expand(index);
}


