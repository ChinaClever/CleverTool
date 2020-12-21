#include "inofmanaging.h"

/**
 * @brief init_update_cmd  初始化升级命令
 * @param addr
 * @param cmd
 */
void init_update_cmd(int addr,updateCmd *cmd)
{
    cmd->header = 0x7B;
    cmd->stx = 0xA5;
    cmd->addr = addr;
    cmd->len = 0x10;

    memset(cmd->reserved,0,sizeof(cmd->reserved)); //清零

    cmd->XOR = 0;  //异或
    cmd->XOR ^=cmd->header;
    cmd->XOR ^=cmd->stx;
    cmd->XOR ^=cmd->addr;
    cmd->XOR ^=cmd->len;

    //    cmd->XOR ^=cmd->reserved;
}

/*
 * addr 执行板地址 array 数据打包后
 * 该函数用于将需要发送的远程升级命令打包
 */
void send_to_packet(uchar addr,QByteArray &array)
{
    updateCmd cmd;

    init_update_cmd(addr,&cmd);

    array.append(cmd.header);
    array.append(cmd.stx);
    array.append(cmd.addr);
    array.append(cmd.len);

    for(int i = 0 ; i < sizeof(cmd.reserved);i++)
        array.append(cmd.reserved[i]);

    array.append(cmd.XOR);
}

void init_text_cmd(uchar addr ,char *data,textSendCmd *cmd)//数据发送-初始化
{
    cmd->header = 0x7B;
    cmd->addr = addr;
    cmd->len = TEXT_MAX_LEN;
    memcpy(cmd->data,data,TEXT_MAX_LEN); //内存拷贝
}

/**
 * @brief text_send_packet 每1024字节为一包，发送升级文件
 * @param data 一包（升级文件）
 * @param array 调用串口发送时的数据包
 */
void text_send_packet(uchar addr ,char *data,QByteArray &array)
{
    textSendCmd textCmd;

    init_text_cmd(addr,data ,&textCmd);

    array.append(textCmd.header);
    array.append(textCmd.addr);
    array.append((textCmd.len >> 8));
    array.append((textCmd.len & 0xFF));

    for(int i = 0 ; i < TEXT_MAX_LEN ; i++)
        array.append(textCmd.data[i]);

    textCmd.XOR = CRC16_2(array.data(), array.size());
    array.append(textCmd.XOR & 0xFF); //低位在前
    array.append(textCmd.XOR>>8);  //高位在后

    //array.append(textCmd.XOR);
}





void text_send_packet(uchar addr, char *data, QByteArray &array, int len)
{
    textSendCmd textCmd;

    //init_text_cmd(addr,data ,&textCmd);

    textCmd.header = 0x7B;
    textCmd.addr = addr;
    textCmd.len = len;
    memcpy(textCmd.data,data, len); //内存拷贝

  //  qDebug() << "len" << textCmd.len;
    array.append(textCmd.header);
    array.append(textCmd.addr);
    array.append((textCmd.len >> 8));
    array.append((textCmd.len & 0xFF));



    for(int i = 0 ; i < len ; i++)
        array.append(textCmd.data[i]);

    //假设在补充在后
    uchar dat = 0x00;
    for(int i = 0 ; i < TEXT_MAX_LEN - len ; i++)
        array.append(dat);

    textCmd.XOR = CRC16_2(array.data(), array.size());
   // qDebug() << "getCRC" <<  CRC16_2(array.data(), array.size());
    array.append(textCmd.XOR & 0xFF); //低位在前
    array.append(textCmd.XOR>>8);  //高位在后

}

void text_change_send_packet(uchar addr,uchar funCode,ushort reg,ushort content, QByteArray &array)
{
    array.append(addr);
    array.append(funCode);
    array.append(reg>>8);
    array.append(reg& 0xFF);
    array.append(content>>8);
    array.append(content& 0xFF);
    unsigned short xorValue = CRC16_2(array.data(), array.size());
   // qDebug() << "getCRC" <<  CRC16_2(array.data(), array.size());
    array.append(xorValue& 0xFF); //低位在前
    array.append(xorValue>>8);  //高位在后
}
