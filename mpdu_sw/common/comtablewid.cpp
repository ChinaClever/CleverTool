/*
 * tableWidget公共蕨类
 *     提供接口，protect所有方法
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "comtablewid.h"
#include "ui_comtablewid.h"
#include <QGridLayout>

ComTableWid::ComTableWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComTableWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);

//    QGridLayout *gridLayout = new QGridLayout(parent);
//    gridLayout->setContentsMargins(0, 0, 0, 0);
//    gridLayout->addWidget(this);
}

ComTableWid::~ComTableWid()
{
    delete ui;
}


/**
 * @brief 初始化窗口
 */
void ComTableWid::initTableWidget(QStringList &header)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);        //设置行数/

    ui->tableWidget->setColumnCount(header.size());    //设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ui->tableWidget->setColumnHidden(0, true);
    // ui->tableWidget->setColumnWidth(0,200);
}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void ComTableWid::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(row, column, item);
}

/**
 * @brief 增加一行内容
 * @param list
 */
void ComTableWid::addRowContent(QStringList &list)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    // ui->tableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}


/**
 * @brief 初始化一行表格
 * @param str
 */
void ComTableWid::addInitRow()
{
    QStringList list;
    int column = ui->tableWidget->columnCount();
    for(int j=0; j<column; ++j)
        list << "---";
    addRowContent(list);
}

/**
 * @brief 初始化表格
 * @param header 列表头
 * @param line 表格行数
 * @param title 表格名称
 */
void ComTableWid::initTableWid(QStringList &header, int line, const QString &title)
{
    initTableWidget(header);
    for(int i=0; i<line; ++i)
        addInitRow();
    ui->groupBox->setTitle(title);
}

/**
 * @brief 修改表格Item
 * @param id  行号
 * @param column 列号
 * @param str 内容
 */
void ComTableWid::setTableItem(int id, int column, const QString &str)
{
    QTableWidgetItem *item = ui->tableWidget->item(id, column);
    item->setText(str);
}

/**
 * @brief 表格行数重新调整
 * @param line 目标行数
 */
void ComTableWid::checkTableRow(int line)
{
    int row = ui->tableWidget->rowCount();
    if(row < line)
    {
        for(int i=0; i<line-row; ++i)
            addInitRow();
    }
    else if(row > line)
    {
        for(int i=0; i<row-line; ++i)
            ui->tableWidget->removeRow(line);
    }
}

/**
 * @brief 清除一行内容
 * @param row 行号
 */
void ComTableWid::clearRow(int row)
{
    int column = ui->tableWidget->columnCount();
    for(int i=0; i<column; ++i) {
         setTableItem(row, i, "---");
    }
}

/**
 * @brief 清除表格所有内容
 */
void ComTableWid::clearTable()
{
    int row = ui->tableWidget->rowCount();
    for(int i=0; i<row; ++i)
        clearRow(i);
}

/**
 * @brief 设置Item的颜色
 * @param id 行号
 * @param column 列号
 * @param alarm 颜色号
 */
void ComTableWid::setItemColor(int id, int column, int alarm)
{
    QTableWidgetItem *item = ui->tableWidget->item(id, column);

    switch (alarm) {
    case 0:
        item->setTextColor(QColor(Qt::black));
        break;
    case 1:
        item->setTextColor(QColor(Qt::yellow));
        break;
    case 2:
        item->setTextColor(QColor(Qt::red));
        break;
    default:
        break;
    }
}
