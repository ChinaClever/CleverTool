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
    mTestStep = 1;
    mStep = 1;
    start();
}

//void TestCoreThread::updateData(int i)
//{
//    //mTrans->sentStep(i);
//    sleep(5);
//}

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
    mTestStep++;
    item.result = str;
    mItem->dataItem.append(item);
    conditionExec(item.status);

    QString statusStr = item.subItem + "  结果: " + str;
    updateProgress(item.status, statusStr);

    return item.status;
}

void TestCoreThread::delayItem(sTestDataItem &item, int s)
{
    item.subItem = tr("延时%1ms").arg(s);
    item.expect = "";
    item.measured = "";
    item.status = true;
    appendResult(item);
    msleep(s);
}


bool TestCoreThread::testReady()
{
    bool ret = true;
    sTestDataItem item;

    item.item = tr("测试前准备");
    delayItem(item, 5*1000);//
    return ret;
}

bool TestCoreThread::testResis( QString & recv)
{
    bool ret = false;
    sTestDataItem item;
    QString sendStr = "";
    mStep = 1;

    item.item = tr("接地电阻测试准备");
    mTestStep = 1;
    mTrans->sentStep(mStep , mTestStep , sendStr);//*IDN?+回车 连接命令 1
    delayItem(item, 1000);//mTestStep++ 2
    mTestStep = 3;
    {
        item.subItem = tr("复位");

        recv = mTrans->sentStep(mStep , mTestStep ,sendStr);//FB+回车 3

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);//mTestStep++
    }

    delayItem(item, 1000);//mTestStep++ 4
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入记忆组1");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S51+回车 5

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 500);//mTestStep++ 6

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入测试步骤1");

        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S61+回车 7

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    item.item = tr("接地电阻参数设置");
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入参数设置功能");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FF+回车 8

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("输出电流");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SY25+回车 9

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("输出电压");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SZ8+回车 10

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("测试时间");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S260+回车 11

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("电阻上限");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S0100+回车 12

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("电阻下限");
        recv = mTrans->sentStep(mStep , mTestStep, sendStr);//S10+回车 13

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("频率50Hz");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FP+回车 14

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("关于扫描器");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S30+回车 15

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("测线电阻归零");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S424+回车 16

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("关闭记忆步骤连接");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FR+回车 17

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("接地电阻测试");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FA+回车 18

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }

    delayItem(item, 10*1000);//mTestStep++ 19

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("读取接地电阻测试结果");
        recv = mTrans->sentStep(mStep , mTestStep ,sendStr);//?K+回车 20

        ret = item.status = getGndValue(recv);
        item.expect = "0-100mΩ";
        item.measured = recv;

        appendResult(item);
    }
    return ret;
}

bool TestCoreThread::testInsulation(QString & recv)
{
    bool ret = false;
    sTestDataItem item;
    QString sendStr = "";
    mStep = 2;

    mTestStep = 1;
    item.item = tr("绝缘电阻测试准备");
    recv = mTrans->sentStep(mStep , mTestStep , sendStr);//*IDN?+回车 连接命令 1
    delayItem(item, 1000);//mTestStep++ 2
    mTestStep = 3;
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("复位");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FB+回车 3

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);//mTestStep++
    }

    delayItem(item, 1000);//mTestStep++ 4
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入记忆组1");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S51+回车 5

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 500);//mTestStep++ 6

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入测试步骤2");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S62+回车 7

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    item.item = tr("输入对地绝缘测试参数设置");
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入参数设置功能");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FE+回车 8

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("绝缘耐压");
        recv = mTrans->sentStep(mStep , mTestStep ,sendStr);//SS500+回车 9

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("绝缘电阻最低充电电流");
        recv = mTrans->sentStep(mStep , mTestStep ,  sendStr);//ST0+回车 10

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("绝缘电阻上限");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SU0+回车 11

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("绝缘电阻下限");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SV2+回车 12

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("测试时间");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SW3+回车 13

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("关闭记忆步骤连接");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FR+回车 14

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("绝缘电阻测试");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FA+回车 15

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 3000);//mTestStep++ 16

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("读取绝缘电阻测试结果");

        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//?K+回车 17

        item.expect = "2-9999MΩ";
        ret = item.status = getIRValue(recv);
        item.measured = recv;
        appendResult(item);
    }
    return ret;
}

bool TestCoreThread::testWithstand(QString & recv)
{
    bool ret = false;
    sTestDataItem item;
    QString sendStr = "";
    mStep = 3;

    mTestStep = 1;
    item.item = tr("输入对地耐压测试准备");
    recv = mTrans->sentStep(mStep , mTestStep , sendStr);//*IDN?+回车 连接命令 1
    delayItem(item, 1000);//mTestStep++ 2
    mTestStep = 3;
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("复位");
        recv = mTrans->sentStep(mStep , mTestStep ,sendStr);//FB+回车 3

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);//mTestStep++
    }

    delayItem(item, 1000);//mTestStep++ 4
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入记忆组1");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S51+回车 5

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 500);//mTestStep++ 6

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入测试步骤3");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//S63+回车 7

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    item.item = tr("输入对地耐压参数设置");
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("进入参数设置功能");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FD+回车 8

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("直流电压");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SI2121+回车 9

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("漏电流上限");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SJ5000+回车 10

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }

    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("漏电流下限");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SK0+回车 11

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("缓升时间");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SL6+回车 12

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("持续时间");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SM3+回车 13

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 3000);//mTestStep++ 14
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("直流最低充电电流");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SO零+回车 15 OK的O加上1234567890的0

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("开启直流缓动电流");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//F8+回车 16

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("直流电弧敏感度");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SP3+回车 17

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("关闭直流电弧判定模式");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FN+回车 18

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("直流测线漏电流归零");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//SR零+回车 19

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("关闭记忆步骤连接");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FR+回车 20

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("输入对地耐压测试");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FA+回车 21

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);
    }
    delayItem(item, 9000);//mTestStep++ 22
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("读取输入对地测试结果");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//?K+回车 23

        item.expect = "0-9999μA";
        ret = item.status = getDCWValue(recv);
        item.measured = recv;
        appendResult(item);
    }

    return ret;
}

bool TestCoreThread::testEnd()
{
    bool ret = true;
    sTestDataItem item;
    QString sendStr = "";
    QString recv = "";
    mStep = 1;
    mTestStep = 3;

    item.item = tr("测试结束");
    {
        sendStr.clear();
        recv.clear();
        item.subItem = tr("复位");
        recv = mTrans->sentStep(mStep , mTestStep , sendStr);//FB+回车 3

        item.expect = sendStr;
        item.measured = recv;
        item.status = sendStr == recv?true:false;
        appendResult(item);//mTestStep++
    }

    {
        item.subItem = tr("结束");
        item.expect = "";

        item.measured = "";
        item.status = true;
        appendResult(item);//mTestStep++
    }

    return ret;
}

bool TestCoreThread::getGndValue(QString& str)
{
    bool ret = false;
    if(str.isEmpty()) return ret;
    QStringList list = str.split(" ");
    QString tempStr = str;
    if( tempStr.contains("Pass") )
    {
        str = list.at(list.size()-1);
        str = str.left(str.indexOf('m'));
        if(str.at(0)==">")str = str.right(str.size()-1);
        if(str.toDouble() >= 0 && str.toDouble() <= 100)
            ret = true;
        else
            ret = false;//false
        str += "mΩ";
        //qDebug()<<"Gndqstring"<<str;
    }
    return ret;
}

bool TestCoreThread::getIRValue(QString& str)
{
    bool ret = false;
    if(str.isEmpty()) return ret;
    QStringList list = str.split(" ");
    QString tempStr = str;
    if( tempStr.contains("Pass") )
    {
        str = list.at(list.size()-1);
        str = str.left(str.indexOf('M'));
        if(str == "----")
            ret = false;//false
        else if(str.at(0) == ">")
        {
            str = str.right(str.size()-1);
            ret = (str.toInt()==9999)?true:false;
        }
        else if(str.toDouble() >= 2 && str.toDouble() <= 9999)
            ret = true;
        str += "MΩ";
        //qDebug()<<"IRqstring"<<str;
    }

    return ret;
}

bool TestCoreThread::getDCWValue(QString& str)
{
    bool ret = false;
    if(str.isEmpty()) return ret;
    QStringList list = str.split(" ");
    QString tempStr = str;
    if( tempStr.contains( "Pass") )
    {
        str = list.at(list.size()-1);
        str = str.left(str.indexOf('A')-1);
        if(str.toDouble() >= 0 && str.toDouble() <= 9999)
            ret = true;
        else
            ret = false; //false

        str = QString::number(str.toDouble()/1000.0,'f',4)+"mA";
        //qDebug()<<"DCWqstring"<<str;
    }
    return ret;
}

void TestCoreThread::run()
{
    testReady();

    sSnItem *sn = &(mItem->sn);
    QString recv = "";
    bool ret = false;
    if(sn->isGnd)//选择接地电阻
    { // 接地电阻测试
        ret =testResis(recv);
        sn->gnd = recv; // 接地电阻测试结果

        if(ret)
        {
            if(sn->isIr)//选择绝缘电阻
            {  // 绝缘电阻测试
                ret = false;
                ret = testInsulation(recv);
                sn->ir = recv; // 绝缘电阻测试结果
                if(ret)
                {
                    if(sn->isDcw)//选择直流压耐
                    {  // 直流压耐测试
                        ret = false;
                        ret = testWithstand(recv);
                        sn->dcw = recv; // 直流压耐测试结果
                    }
                }
            }
            else//没有选择绝缘电阻
            {
                if(sn->isDcw)
                {  // 直流压耐测试
                    ret = false;
                    ret = testWithstand(recv);
                    sn->dcw = recv; // 直流压耐测试结果
                }
            }
        }
    }
    else//没有选择接地电阻
    {
        if(sn->isIr)//选择绝缘电阻
        {  // 绝缘电阻测试
            ret = false;
            ret =testInsulation(recv);
            sn->ir = recv; // 绝缘电阻测试结果
            if(ret)
            {
                if(sn->isDcw)//选择直流压耐
                {  // 直流压耐测试
                    ret = false;
                    ret =testWithstand(recv);
                    sn->dcw = recv; // 直流压耐测试结果
                }
            }
        }
        else//没有选择绝缘电阻
        {
            if(sn->isDcw)
            {  // 直流压耐测试
                ret = false;
                ret = testWithstand(recv);
                sn->dcw = recv; // 直流压耐测试结果
            }
        }
    }
    testEnd();
    emit overSig();
}
