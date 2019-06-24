/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "snmpthread.h"
#include <cassert>

SnmpThread::SnmpThread(QObject *parent) : QThread(parent)
{
    isRun = mFlag = mSetOrRead = false;//mSetOrRead false: read  true:set
    mMutex = new QMutex();
    mSnmpRes = new sSnmpRes();

    m_snmp_client =  new QtSnmpClient(this);
    m_snmp_client->setCommunity( "public" );
    connect( m_snmp_client,SIGNAL(responseReceived(qint32,QtSnmpDataList)), this, SLOT(onResponseReceived(qint32,QtSnmpDataList)) );
    connect( m_snmp_client, SIGNAL(requestFailed(qint32)), this, SLOT(onRequestFailed(qint32)) );

    m_timer =  new QTimer(this);
    connect( m_timer, SIGNAL(timeout()), SLOT(timeoutDone()));

    mSetTimer =  new QTimer(this);
    connect( mSetTimer, SIGNAL(timeout()), SLOT(setSlot()) );
}

SnmpThread::~SnmpThread()
{
    isRun = false;
    wait();
}

void SnmpThread::startRead(const QString &addr, int msec)
{
    if(!isRun)
    {
        if(msec == 0) msec = 500 + (rand() % 1000);
        if(!m_timer->isActive()) m_timer->start(msec);
        if(!mSetTimer->isActive()) mSetTimer->start(mSendTimer);
        memset(mSnmpRes, 0, sizeof(sSnmpRes));
        mSnmpRes->timeoutmsec = msec;
        m_snmp_client->setAgentAddress(QHostAddress(addr));
        m_address = addr;
        m_snmp_client->cancelWork();
        isRun = true;
    }
}


void SnmpThread::stopRun()
{
    if(isRun)
    {
        isRun = false;
        if(m_timer->isActive())  m_timer->stop();;
        if(mSetTimer->isActive()) mSetTimer->stop();

        mOids.clear();
        mSubOid.clear();
    }
}    

void SnmpThread::setValue(QList<sSnmpSetCmd> &cmd , int timer)
{
    mSetCmdList = cmd;
    mSendTimer = timer;
}

void SnmpThread::setSlot()
{
    if(mSetOrRead)
    {
        if(!mFlag)
        {
            if(mSetCmdList.size())
            {
                if( ! m_snmp_client->isBusy() ) {
                    sSnmpSetCmd cmd = mSetCmdList.first();
                    mSetCmdList2.append(cmd);
                    m_snmp_client->setValue("private", cmd.oid, cmd.type, cmd.value);
                    //mSnmpRes->reqmsec = QTime::currentTime().msecsSinceStartOfDay();
                    //qDebug()<<"setSlot"<<cmd.oid<<cmd.type<<cmd.value;
                    emit requestSig(cmd.oid);
                    mSetCmdList.removeFirst();
                    getResTime();
                }

            }
            else
            {
                mFlag = true;
                mSetOrRead = false;
            }
        }
        else
        {
            if(mSetCmdList2.size())
            {
                if( ! m_snmp_client->isBusy() ) {
                    sSnmpSetCmd cmd = mSetCmdList2.first();
                    mSetCmdList.append(cmd);
                    m_snmp_client->setValue("private", cmd.oid, cmd.type, cmd.value);
                    //mSnmpRes->reqmsec = QTime::currentTime().msecsSinceStartOfDay();
                    //qDebug()<<"setSlot"<<cmd.oid<<cmd.type<<cmd.value;
                    emit requestSig(cmd.oid);
                    mSetCmdList2.removeFirst();
                    getResTime();
                }

            }
            else
            {
                mFlag = false;
                mSetOrRead = false;
            }
        }
    }
}

void SnmpThread::getReqTime()
{
    mSnmpRes->resmsec = QTime::currentTime().msecsSinceStartOfDay();
    int msec = mSnmpRes->resmsec - mSnmpRes->reqmsec;
    if(msec > 0) {
        if(msec > mSnmpRes->timeoutmsec)
            mSnmpRes->out++;
        else
            mSnmpRes->ok++;
    } else {
        mSnmpRes->ok++;
    }

    // 最长响应时间
    if(msec > mSnmpRes->longmsec)
        mSnmpRes->longmsec = msec;

    // 计算平均响应时间
#if 1
    if(mSnmpRes->all) {
        msec +=  mSnmpRes->risimsec;
        mSnmpRes->risimsec = msec / 2;
        if(mSnmpRes->risimsec == 0) mSnmpRes->risimsec = 1;
    } else {
        mSnmpRes->risimsec = msec;
    }
#else
    if(mSnmpRes->all) {
        int n = mSnmpRes->all;
        mSnmpRes->risimsec =(int) ((double)(n-1)/n*mSnmpRes->risimsec + msec/n);
    }
#endif

    mSnmpRes->reqmsec = 0;
}

void SnmpThread::onResponseReceived(const qint32, const QtSnmpDataList& values )
{
    QMutexLocker locker(mMutex);
    getReqTime();

    emit responseSig(values);
    for( const auto& value : values ) {
        qDebug( "%s | %s : %s\n", qPrintable( "192" ),  qPrintable( value.address() ),  qPrintable( value.data()) );
    }
}

void SnmpThread::onRequestFailed( const qint32 request_id )
{
    qDebug() << request_id;
    mSnmpRes->err++;
    mSnmpRes->reqmsec = 0;
    emit reqErrSig();
}

bool SnmpThread::makeRequest()
{    
    bool ret = true;

    if(mOids.size()) {
        //if(! m_snmp_client->isBusy()){
        m_snmp_client->requestValues(mOids);
        emit requestSig(mOids.first());
        //}
    }else if(!mSubOid.isEmpty()) {
        //if(! m_snmp_client->isBusy()){
        m_snmp_client->requestSubValues(mSubOid);
        emit requestSig(mSubOid);
        mSetOrRead = true;
        //}
    } else {
        ret = false;
    }

    return ret;
}


void SnmpThread::getResTime()
{
    mSnmpRes->all++;
    mSnmpRes->reqmsec = QTime::currentTime().msecsSinceStartOfDay();
}

bool SnmpThread::checkRes()
{
    bool ret = false;
    if(!m_snmp_client->isBusy()) {
        if(0 == mSnmpRes->reqmsec) ret = true;
    }

    return ret;
}

void SnmpThread::timeoutDone()
{
    if(!mSetOrRead)
    {
        if(checkRes()) {
            if(makeRequest()) {
                getResTime();
            }
        }
    }
}


