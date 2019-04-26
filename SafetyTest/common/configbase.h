#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"
#include "serialportdlg.h"

enum {
    Test_Stop = 0,
    Test_SNMP,  // 模拟测试
    Test_Rtu,

    Test_Function, // 功能测试
    Test_Start,
    Test_Pause,
    Test_Continue,
    Test_Over,

    Test_Result_Wid,
    Test_Items_Wid,
    Test_Datas_Wid,

    Info_Line, // 相信息信息
    Info_Trans, // Modbus传输统计实时显示界面
    Info_LineThreshold, // 设备阈值实时显示界面
    Info_Env, // 环境信息_
    Info_Loop,
    Info_LoopThreshold,
    Info_Output,
    Info_OutputThreshold,
    Info_Set,

    Log_Modbus, // Modbus日志界面
    Log_Trans, // Modbus 传输日志
    Log_Env,  //设备环境日志
    Log_Line, // 设备纪录日志
    Log_LineThreshold, // 设备阈值日志
    Log_Loop,
    Log_Output,
    Log_Alarm,
};


struct sConfigItem
{
    sConfigItem() {testMode=0; msecs = 5; logMins=60;}

    int testMode; // 测试模式
    int setMode;
    int msecs;
    int logMins; // 日志间隔时间分钟

    QString ip;
    SerialPort *serial; // 串口对象
    int devNum; // 级联数
    int lineNum;
    int outputNum;
    int cmdModel; // 命令模式
    int v;
};

class ConfigBase
{
public:
    ConfigBase();
    sConfigItem *item;

    QString getSerialName();
    void setSerialName(const QString &name);

    int getTestMode();
    void setTestMode(int mode);

    QString getIp();
    void setIp(const QString &ip);

    int getLineNum();
    void setLineNum(int num);

    int getDevNum();
    void setDevNum(int num);

    int getModbusCmd();
    void setModbusCmd(int num);

    int getModbusTime();
    void setModbusTime(int num);

    int getLogTime();
    void setLogTime(int num);

    int getOutputNum();
    void setOutputNum(int num);

    int getVersion();
    void setVersion(int num);

    int getResistance(int addr,int bit);
    void setResistance(int addr,int bit,int num);

    virtual QString getPrefix()=0;
};

#endif // CONFIGBASH
