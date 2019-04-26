/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "excel_savethread.h"
#include "msgbox.h"

Excel_SaveThread::Excel_SaveThread(QObject *parent) : QThread(parent)
{

}

Excel_SaveThread::~Excel_SaveThread()
{
    wait();
}

void Excel_SaveThread::saveData(const QString &fileName, QList<QStringList> &list)
{
    mFileName = fileName;
    mList.append(list);
    start();
}

bool Excel_SaveThread::init()
{
    bool ret = true;

    HRESULT r = OleInitialize(0);  // 增加头文件 #include <sapi.h>
    if (r != S_OK && r != S_FALSE)
    {
        qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);
        CriticalMsgBox box(0, tr("Qt:初始化Ole失败"));
        ret = false;
    } else {
        mSize = 0;
        mAllSize = 1;
    }
    return ret;
}

int Excel_SaveThread::getProgress()
{
    int ret = 100;

    if(mSize < mAllSize)
        ret = (mSize *100.0) / mAllSize ;

    return ret;
}

void Excel_SaveThread::writeFile(QList<QStringList> &list)
{
    QExcel excel(this);
    bool ret = excel.createNewExcel(mFileName +".xlsx");
    if(ret) {
        mAllSize = mList.size();
        excel.selectSheet("Sheet1"); //  Sheet1
        excel.saveAs();

        for(int i=0; i<list.size(); ++i)
        {
            for(int j=0; j<list.at(i).size(); ++j) {
                excel.setCellString(i+1, j+1, list.at(i).at(j));
            }
            msleep(15);
        }
        excel.save();
    }
    list.clear();
}


void Excel_SaveThread::workDown()
{
    bool ret = init();
    if(ret) {
        writeFile(mList);
    }
}

void Excel_SaveThread::run()
{
    workDown();
}
