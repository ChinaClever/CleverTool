/*
 * sqltablemodel.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QWidget *parent) :
    QWidget(parent)
{
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setSort(0, Qt::AscendingOrder); //选择按照 第一列 排序
}



void SqlTableModel::setHeaders(QStringList &head)
{
    for(int i=0; i<head.size(); ++i)
        model->setHeaderData(i, Qt::Horizontal, head[i]);
}

/**
 * @brief 提交修改
 * @return
 */
bool SqlTableModel::submitChanges(void)
{
    bool ret = true;
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
        model->database().commit(); //提交
    } else {
        model->database().rollback(); //回滚
        ret = false;
        qDebug() << tr("数据库错误: %1").arg(model->lastError().text());
    }
    return ret;
}

/**
 * @brief 撤销修改
 */
void SqlTableModel::revertChanges(void)
{
    model->revertAll();
}

/**
 * @brief 删除行
 * @param curRow
 * @return
 */
bool SqlTableModel::removeRow(int curRow)
{
    bool ret = model->removeRow(curRow);
    if(ret)
        ret = submitChanges();

    return ret;
}


/**
 * @brief 查询字段
 * @param str
 */
void SqlTableModel::queryFilter(QString str)
{
    model->setFilter(str); //进行筛选
    model->select(); //显示结果
}


/**
 * @brief 重新关联表
 */
bool SqlTableModel::refreshTable(const QString &name)
{
    model->setTable(name); //重新关联表
    model->select(); //这样才能再次显示整个表的内容

    QString tableName = model->tableName();
    if(name != tableName) {
        qDebug() << "refresh Table"<< name << "err";
        return false;
    }

    return true;
}

/**
 * @brief 升序排列
 * @param id
 */
void SqlTableModel::ascendingOrder(int id)
{
    model->setSort(id,Qt::AscendingOrder);
    model->select();
}

/**
 * @brief 降序
 * @param id
 */
void SqlTableModel::descendingOrder(int id)
{
    model->setSort(id,Qt::DescendingOrder);
    model->select();
}
