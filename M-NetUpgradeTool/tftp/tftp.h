#ifndef TFTP_H
#define TFTP_H

#include "common\datapacket.h"
#include <QUdpSocket>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QtEndian>
#include <QString>
#include <QProgressBar>
#include <QStyleFactory>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <sys/ioctl.h>
#include <assert.h>

#define OPCODE_RRQ (1)
#define OPCODE_WRQ (2)
#define OPCODE_DATA (3)
#define OPCODE_ACK (4)
#define OPCODE_ERR (5)

#define BLOCKSIZE (512)
#define DATA_PACKET_HEADER_LEN (4)
#define RECV_BUFFER_SIZE 1024

class Tftp : public QThread
{
    Q_OBJECT
public:
    explicit Tftp(QObject *parent = 0);

    bool upload(const QString &file, const QString &ip, int port=69, int sec=60);
    bool getload(const QString &file, const QString &ip, int port=69);

    void breakDown();
    void startDown();

protected:
    void sendReadReqMsg(char *pFilename);
    void sendDataAckMsg(struct TFTPData *pData, QHostAddress sender, quint16 senderPort);
    void sendWriteReqMsg(char *pFilename);
    void sendDataMsg(short blockno, QHostAddress sender, quint16 senderPort);

signals:
    void progressSig(float, QString);

private slots:
    void readPendingDatagrams();

private:
      char recvData[RECV_BUFFER_SIZE];
      QUdpSocket *udpSocketClient;
      QFile *rFile;
      QFile *sFile;
      bool isRun;
      int mPort;
};


struct TFTPHeader{
    short opcode;
}__attribute__((packed));

struct TFTPWRQ{
    struct TFTPHeader header;
    char *filename;
    char *mode;
}__attribute__((packed));

struct TFTPData{
    struct TFTPHeader header;
    short block;
    char data[];
}__attribute__((packed));

struct TFTPACK{
    struct TFTPHeader header;
    short block;
}__attribute__((packed));

struct TFTPERR{
    struct TFTPHeader header;
    short errcode;
    char *errmsg;	// Terminal as \0x00
}__attribute__((packed));


#endif // TFTP_H
