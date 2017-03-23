#ifndef DATADEFINE_H
#define DATADEFINE_H
#include<qglobal.h>


//#define uchar unsigned char

quint8 sentCmd[2][4] ={
    {0x04,  0x04,  0x01,  0x04},
    {0x05,  0x06,  0x01,  0x04}
};

quint8 recalibrateCmd[16]={0x7B , 0xA0 , 0x10 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 00 , 0xCB};
quint8 recalibrateBack[7]={0xA0 ,0x10 ,0xAA ,0xAA ,0xAA ,0xBB ,0xA1};

 quint8 recvCmd[2][5] ={
    {0x04,  0xCC,  0xCC,  0xCC,  0xBB},
    {0x05,  0xDD,  0xDD,  0xDD, 0xBB}
};

quint8 transCmd[4][6]={
    {0x01,  0x04,  0x04,  0x01,  0x04,  0x04 },
    {0x02,  0x04,  0x04,  0x01,  0x04,  0x07 },
    {0x03,  0x04,  0x04,  0x01,  0x04,  0x06 },
    {0x04 , 0x04,  0x04,  0x01,  0x04,  0x01 }
};

quint8 phaseCmd[4][6]={
    {0x01,  0x05,  0x06,  0x01,  0x04,  0x07 },
    {0x02,  0x05,  0x06,  0x01,  0x04,  0x04 },
    {0x03,  0x05,  0x06,  0x01,  0x04,  0x05 },
    {0x04,  0x05,  0x06,  0x01,  0x04,  0x02 }
};

typedef struct responedData{
    quint8 addr;    //地址码
    quint8 cmd[5];  //发送命令
    quint8 xornumber;   //校验码
}recvdata;

typedef struct responseCollected{
    quint8 head[2]; //头码
    quint8 addr;    //地址位
    quint8 length;  //长度
    quint8 flag;    //标志位，00:表示只读取电流电压，A1:表示读取电流电压并可以控制继电器
    quint8 openeffective[6];   //开启有效
    quint8 closeffective[6];   //关闭有效
    quint8 obligate[3];        //预留
    quint8 xornumber;          //校验码
}collectedData;

typedef struct returntableData{
   quint8 sign;            //执行板返回标志
   quint8 addr;            //执行板地址
   quint8 len;             //长度
   quint8 onoffState;      //八位开关状态
   quint8 current[8][2];   //电流，第一位到第八位，每位两个字节
   quint8 volate[2][2];    //电压，第一个和第八个
   quint8 power[2][2];     //功率，前四位和后四位总功率
   quint8 xornumber;       //校验码
}returndata;

recvdata recv;//应答

#endif // DATADEFINE_H
