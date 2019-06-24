#ifndef SI_RTUTHREAD_H
#define SI_RTUTHREAD_H

#include "si_rtu_sent.h"

#define RTU_BUF_SIZE 2*SERIAL_LEN

struct sSetCmd
{
    int addr;
    int fn;
    int reg;
    ushort value;
};

class SI_RtuThread : public QThread
{
    Q_OBJECT
public:
    explicit SI_RtuThread(QObject *parent = nullptr);
    static SI_RtuThread *bulid(QObject *parent = nullptr);
    ~SI_RtuThread();
    void stopThread();

    void init(SerialPort *serial);
    void setCmd(sSetCmd &cmd) {mList.append(cmd);}
    int mreg;
    void setTimer(int timer);
    bool checkData(QByteArray mBuf, bool& ret, QString &ans, int &color, int rtn);

protected:    
    bool sentSetCmd(int addr, int fn, int reg, ushort value, int msecs);
    void run();

signals:
    void cmdSig(QString,int);
    void countSig(int);

public slots:

private:
    bool isRun;
    uchar *mBuf;
    uchar *mBuf2;
    SerialPort *mSerial;
    QMutex *mMutex;
    SI_RtuSent *mRtuSent;

    QList<sSetCmd> mList;
    int m_Timer;
};

#endif // SI_RTUTHREAD_H
