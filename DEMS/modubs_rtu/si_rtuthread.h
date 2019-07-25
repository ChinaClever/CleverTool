#ifndef SI_RTUTHREAD_H
#define SI_RTUTHREAD_H

#include "si_rtu_sent.h"
#include"common/excel/excel_savethread.h"
#define RTU_BUF_SIZE 2*SERIAL_LEN

struct sSetCmd
{
    int addr;
    int fn;
    int reg;
    ushort value;
};

#define RTU_LINE_NUM 9 // 3相
#define RTU_TH_NUM 3 // 3个传感器
//定制
#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11+6)  // 长度需要改变 ////============ 加上开关，功率因素之后，是为14 [追加交直流区分]

//原来
//#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11)  // 长度需要改变 ////============ 加上开关，功率因素之后，是为14 [追加交直流区分]
#define RTU_SENT_DC_LEN (22*4+1+3*3+1+1+3+5)  // 长度需要改变 直流长度 [追加交直流区分
#define N_Line  3
struct RtuRecvLine {
    ushort vol; // 电压值
    ushort cur; // 电流值
    ushort pow; // 功率值
    ushort apPow; // 视在功率
    uint ele; // 电能值

    ushort minVol; // 最小电压
    ushort maxVol; // 最大电压
    ushort minCur; // 最小电流
    ushort maxCur; // 最大电流

    ushort wave; // 谐波值
    uchar pf; // 功率因素
    uchar sw; // 开关
};

struct RtuEnvUnit {
    uchar value;
    uchar min;
    uchar max;
};

struct RtuRecvEnv {
    RtuEnvUnit tem; // 温度
    RtuEnvUnit hum; // 湿度
};

struct Rtu_recv {
    uchar addr; // 表示从机地址码
    uchar fn; // 表示功能码
    ushort len; // 表示数据字节数
    uchar version;

    RtuRecvLine data[RTU_LINE_NUM];
    RtuRecvEnv  env[RTU_TH_NUM];
    uchar lineNum;
    uchar rate;
    uchar dc; // 交直流标志位

    uchar lps; // 防雷开关
    uchar pl[3]; // 负载百分比
    uchar hc; //谐波通道(00表示A路电压通道，01表示B路电压通道，02表示C路电压通道 03表示A路电流通道，04表示B路电流通道，05表示C路电流通道)
    ushort thd[40]; // 庇波含量

    ushort crc; // 检验码
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
    void setLen(int len);
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
    int mLen;
    Rtu_recv *mRtuPkt ; //传输数据结构
    QStringList mQstringList;
};
bool rtu_recv_packet(uchar *buf, int len, Rtu_recv *pkt);
ushort rtu_crc(uchar *buf, int len);
#endif // SI_RTUTHREAD_H
