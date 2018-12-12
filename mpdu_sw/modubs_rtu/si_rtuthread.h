#ifndef SI_RTUTHREAD_H
#define SI_RTUTHREAD_H

#include "si_rtu_sent.h"

#define RTU_BUF_SIZE 2*SERIAL_LEN

struct sSetCmd
{
    int addr;
    int reg;
    ushort value;
};

class SI_RtuThread : public QThread
{
    Q_OBJECT
    explicit SI_RtuThread(QObject *parent = nullptr);

public:
    static SI_RtuThread *bulid(QObject *parent = nullptr);
    ~SI_RtuThread();

    void init(SerialPort *serial);
    void setCmd(sSetCmd &cmd) {mList.append(cmd);}

protected:    
    bool sentSetCmd(int addr, int reg, ushort value, int msecs);
    void run();

signals:
    void cmdSig(QString);

public slots:

private:
    bool isRun;
    uchar *mBuf;
    SerialPort *mSerial;
    QMutex *mMutex;
    SI_RtuSent *mRtuSent;

    QList<sSetCmd> mList;
};

#endif // SI_RTUTHREAD_H
