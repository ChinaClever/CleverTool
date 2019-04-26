/*
 * 日志窗口公共基类
 *      只需要实现二个纯虚函数即可
 *       virtual BasicSql *getDb()=0;
         virtual void initTable()=0;
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "logcomwid.h"
#include "ui_logcomwid.h"
#include "common.h"
#include "log_exportdlg.h"

LogComWid::LogComWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogComWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化   
}

LogComWid::~LogComWid()
{
    delete ui;
}

/**
 * @brief 设置窗口名称
 * @param title
 */
void LogComWid::setGroupBoxTitle(const QString &title)
{
    ui->groupBox->setTitle(title);
}

/**
 * @brief 列平均分配
 */
void LogComWid::setStretch()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

/**
 * @brief 初始化方法
 */
void LogComWid::initFunSLot()
{
    QString name =  getDb()->tableName();

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能

    model = new SqlTableModel(ui->tableView);
    ui->tableView->setModel(model->model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
//    ui->tableView->setAlternatingRowColors(true);
//    ui->tableView->setStyleSheet("QTableView{ background-color: rgb(202, 232, 234); alternate-background-color: rgb(245, 245, 245); }");
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));

    initTable();
    mDateWidget = new LOG_DateWidget(ui->dateWid);
    mDateWidget->setTable(name);
    connect(mDateWidget,SIGNAL(selectedSig(QString)), model,SLOT(queryFilter(QString)));

    initBtnBar();
}

/**
 * @brief 初始化按钮
 */
void LogComWid::initBtnBar()
{
    mBtnBar = new LogBtnBar(ui->widget);
    connect(mBtnBar,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(mBtnBar,SIGNAL(clearSig()),this,SLOT(clearTableSlot()));
    connect(mBtnBar,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(mBtnBar,SIGNAL(refreshSig()),mDateWidget,SLOT(updateSlot()));
    connect(mBtnBar,SIGNAL(exportSig()),this,SLOT(exportSlot()));
}


/**
 * @brief 刷新表格
 * @param table 表格名
 * @return
 */
bool LogComWid::refreshTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        setColumnHidden(0);
    }
    return  ret;
}

void LogComWid::setColumnHidden(int column)
{
    ui->tableView->setColumnHidden(column, true);
}

void LogComWid::refreshSlot()
{
    initTable();
}

/**
 * @brief 清空数据库
 */
void LogComWid::clearTableSlot()
{
    model->model->setTable("markingtable");
    BasicSql* db = getDb();
    db->clear();
    db->createTable();
    initTable();
    //    if(model->removeRow(0))
    //        QTimer::singleShot(10,this,SLOT(clearTableSlot()));
}

/**
 * @brief 删除一条纪录
 */
void LogComWid::doubleSlot(QModelIndex)
{
    QString str = tr("是否删除这条纪录?");
    QuMsgBox box(this, str);
    bool ret = box.Exec();
    if(ret)
    {
        int curRow = ui->tableView->currentIndex().row();
        model->removeRow(curRow);
    }
}

/**
 * @brief 日志导出
 */
void LogComWid::exportSlot()
{
    LOG_ExportDlg dlg(this);
    QString name = getDb()->tableName();
    dlg.set(name, mHeadList, mTableTile);

    QDate start, end;
    mBtnBar->getDate(start, end);
    dlg.setDate(start, end);
    dlg.exec();
}
