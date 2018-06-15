#ifndef DATASOUCE_H
#define DATASOUCE_H



#include "datadefine.h"
#include <QtGlobal>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

#define Random(x) (rand() % x) //获取0到x之间的随机数
//#define Random(x) (qsrand(QTime(0,0,0).secsTo(QTime::currentTime())) % x) //获取0到x之间的随机数
typedef struct deviceSettings
{
    uchar devType; //1 -ip  2 -rp  3 -mp
    QString devName;
    uchar viceNumber;
    int devLoopNum;
    int devPhaseNum;
    int devOutNum;
    int devSensorNum;
    quint8 entranceGuard1;
    quint8 entranceGuard2;
    quint8 smoke;
    quint8 water;
    quint16 volUpNum;
    quint16 volDownNum;


}devSetings;


class DataSouce
{
public:
    DataSouce();
    ~DataSouce();


public:


    void updateSouceData();

    template<typename T> void initRandomBuf(T baseNum , T socpe, T *buf);
    //    void initRandomBufquint(quint8 baseNum, quint8 socpe, quint8 *buf);

    template<typename T>  void initFixedBuf(T num , T *buf);
    //    void initFixedBufquint(int num , quint8 *buf);

    void initElectricData(electricData &data);
    void initElectricDataSpace(electricData &data);
    void initUnit(unit &data);
    void initUnitSpace(unit &data);
    parameterData *getSouceData();
    void allocateSpace();

    //提供修改原始数据的接口
    void setData(unit *mUnit, int num, quint8 *data);
    void setSwitchData(electricData *data,int num,quint8 status);  //开关的单一设置
    void setAllSwitchData(electricData *data, quint8 status); //开关的统一设置
    //    void setData(unit *mUnit, int num, quint16 *data);

    void renewalSouceData(); //更新数据
    void renewalUnit(unit *mUnit);

    void changeDevSettings();
    quint16 getRandomNumber(quint16 upNum , quint16 downNum);
    void sleep(unsigned int msec);

private:
    parameterData *souceData;





};

extern DataSouce *get_datasouce();
extern devSetings *get_devsetings();

#endif // DATASOUCE_H
