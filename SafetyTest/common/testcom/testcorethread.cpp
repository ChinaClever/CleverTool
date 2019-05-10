/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "testcorethread.h"

TestCoreThread::TestCoreThread(QObject *parent) : QThread(parent)
{
    mItem = TestConfig::bulid()->item;
    mTrans = new TestTransThread(this);
}

TestCoreThread::~TestCoreThread()
{
    quit();
    terminate();
    wait();
}



void TestCoreThread::startThread()
{
    mItemId = 1;
    start();
}

void TestCoreThread::updateData(int i)
{
    mTrans->sentStep(i);
    sleep(5);
}

void TestCoreThread::stopThread()
{
    quit();
    terminate();
    emit overSig();
}

void TestCoreThread::conditionExec(bool s)
{
    if(mItem->mode == Test_Pause) {
        do{
            msleep(1);
        } while(mItem->mode == Test_Pause);
    }

    if(mItem->mode == Test_Over){
        stopThread();
    }

    if(mItem->sn.errStop) {
        if(!s) stopThread();
    }
}

void TestCoreThread::updateProgress(bool status, QString &str)
{
    sTestProgress *p = &(mItem->progress);
    if(status) {
        p->okNum++;
    } else {
        p->errNum++;
    }
    p->finishNum++;
    p->status = tr("测试项:%1  %2").arg(p->finishNum).arg(str);

    msleep(100);
}


bool TestCoreThread::appendResult(sTestDataItem &item)
{
    QString str = tr("失败");
    if(item.status) {
        str = tr("成功");
    }
    item.id = mItemId++;
    item.result = str;
    mItem->dataItem.append(item);
    conditionExec(item.status);

    QString statusStr = item.subItem + "  结果: " + str;
    updateProgress(item.status, statusStr);

    return item.status;
}

void TestCoreThread::delayItem(sTestDataItem &item, int s)
{
    item.subItem = tr("延时%1S").arg(s);
    item.expect = "";
    item.measured = "";
    item.status = true;
    appendResult(item);
    sleep(s);
}


bool TestCoreThread::testReady()
{
    bool ret = true;
    sTestDataItem item;

    item.item = tr("测试前准备");
    delayItem(item, 5);

    {
        item.subItem = tr("设置串口");
        item.expect = "";
        mTrans->sentStep(mItemId);

        item.measured = "";
        item.status = true;
        appendResult(item);
    }
    delayItem(item, 1);

    {
        item.subItem = tr("复位");
        item.expect = "";
        mTrans->sentStep(mItemId);

        item.measured = "";
        item.status = true;
        appendResult(item);
    }

    return ret;
}


void TestCoreThread::run()
{
    testReady();

    sSnItem *sn = &(mItem->sn);
    if(sn->isGnd) { // 接地电阻测试

        sn->gnd = "X"; // 接地电阻测试结果
    }

    if(sn->isIr) {  // 绝缘电阻测试

        sn->ir = "A"; // 绝缘电阻测试结果
    }

    if(sn->isDcw) {  // 直流压耐测试

        sn->dcw = "A"; // 直流压耐测试结果
    }

    if(sn->isAcw) {  // 交流压耐测试

        sn->acw = "A"; // 交流压耐测试结果
    }


    emit overSig();
}
