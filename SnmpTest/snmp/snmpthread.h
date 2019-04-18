#ifndef SNMP_H
#define SNMP_H

#include <QThread>
#include <QScopedPointer>
#include <QMap>
#include <QtSnmpClient.h>
#include <QtCore>

#define MIB_OID_CLEVER  ".1.3.6.1.4.1.30966"

enum{
    SNMP_INTEGER_TYPE = 0x02,
    SNMP_STRING_TYPE = 0x04,
};

struct sSnmpSetCmd
{
    sSnmpSetCmd() {type=SNMP_STRING_TYPE;}
    QString oid;
    ushort type;
    QByteArray value;
};

struct sSnmpRes
{
    int all; // 请求次数
    int ok;  // 正常回复次数
    int err; // 超时响应次数

    int longmsec; // 最长响应时间
    int risimsec; // 平均响应时间
    int timeoutmsec; // 超时时间

    uint reqmsec; // 请求时间
    uint resmsec; // 响应时间
};

Q_DECLARE_METATYPE(sSnmpSetCmd)

class SnmpThread : public QThread
{
    Q_OBJECT
public:
    explicit SnmpThread(QObject *parent = nullptr);
    ~SnmpThread();

    void startRead(const QString &addr, int msec);
    void stopRun();

    sSnmpRes *getSnmpRes() {return mSnmpRes;}
    void requestValue( const QString& oid) {mOids.clear(); mOids.append(oid);}
    void requestValues( const QStringList& oids ) {mOids = oids;}
    void requestSubValues( const QString& oid ) {mSubOid = oid;}

signals:
    void requestSig(const QString& str);
    void responseSig(const QtSnmpDataList& values);
    void reqErrSig();

protected:
    bool makeRequest();
    void getResTime();
    void getReqTime();
    bool checkRes();

private slots:
    void setSlot();
    void onResponseReceived( const qint32 request_id,const QtSnmpDataList& );
    void onRequestFailed( const qint32 request_id );
    void timeoutDone();

private:
    bool isRun;
    QMutex *mMutex;
    QTimer *m_timer;
    QStringList mOids;
    QString mSubOid;

    sSnmpRes *mSnmpRes;
    QtSnmpClient *m_snmp_client;
    QList<sSnmpSetCmd> mSetCmdList;//设置成public，在别的类可以访问
};

#endif // SNMP_H
