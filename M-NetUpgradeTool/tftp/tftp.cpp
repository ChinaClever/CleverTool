/*
 * tftp.cpp
 * tftp上传功能的实现
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftp.h"

#define GET 0
#define PUT 1

int operation = 0;
int serverPort = 0;
int recv_data_bytes = 0;
int send_data_bytes = 0;
int send_file_size = 0;
short wrq_block_no = 0;
bool put_finished_flag = false;

char *get_Filename;
char *put_Filename;
char *fileName;

QString get_filePath, rrq_filename;
QString put_filePath, put_fileName;
QString info;
QString f_info;
QFileInfo put_fileInfo;
QHostAddress tftpServer;

Tftp::Tftp(QObject *parent) : QThread(parent)
{   
    mPort = 7755;
    udpSocketClient = nullptr;
}

void Tftp::startDown()
{
    isRun = true;
    operation = GET;
    serverPort = 0;
    recv_data_bytes = 0;
    send_data_bytes = 0;
    send_file_size = 0;
    wrq_block_no = 0;
    put_finished_flag = false;

    if(udpSocketClient) {
        udpSocketClient->close();
        delete udpSocketClient;
    }

    udpSocketClient = new QUdpSocket(this);
    udpSocketClient->bind(QHostAddress::Any, mPort++);
    connect(udpSocketClient, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    memset(recvData, 0, sizeof(recvData));
}

void Tftp::breakDown()
{
    isRun = false;
    disconnect(udpSocketClient, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Tftp::sendReadReqMsg(char *pFilename)
{
    emit progressSig(0, "Start");

    struct TFTPHeader header;
    header.opcode = qToBigEndian<short>(OPCODE_RRQ);

    int filenamelen = strlen(pFilename) + 1;
    int packetsize = sizeof(header) + filenamelen + 5 + 1;
    char *rrq_packet = (char *)malloc(packetsize);
    char *mode = "octet";

    memcpy(rrq_packet, &header, sizeof(header));
    memcpy(rrq_packet + sizeof(header), pFilename, filenamelen);
    memcpy(rrq_packet + sizeof(header) + filenamelen, mode, strlen(mode) + 1);

    int bytes = udpSocketClient->writeDatagram(rrq_packet,packetsize,tftpServer,serverPort);
    qDebug()<<"RRQ : "<<bytes<<"had been sent!";

    /*according the path and new the file for reveive data*/
    {
        QString temp_path = get_filePath;
        temp_path.append('/');
        temp_path.append(rrq_filename);
        qDebug()<<"file save as : "<< temp_path;

        rFile = new QFile(temp_path);
        if(rFile->open(QIODevice::ReadWrite))
        {
            qDebug("Could not save as the file you get from tftp server!");
            return;
        }

        temp_path.clear();
        rrq_filename.clear();

        qDebug()<< "Get and save the file info : "<< rFile->fileName();
    }

    if(NULL != rrq_packet)
    {
        free(rrq_packet);
        rrq_packet = NULL;
    }
}

void Tftp::sendWriteReqMsg(char *pFilename)
{
    put_finished_flag = false;
    {
        struct TFTPHeader header;
        header.opcode = qToBigEndian<short>(OPCODE_WRQ);
        int filenamelen = strlen(pFilename) + 1;
        int packetsize = sizeof(header) + filenamelen + 5 + 1;
        char *wrq_packet = (char *)malloc(packetsize);
        char *mode = "octet";

        memcpy(wrq_packet, &header, sizeof(header));
        memcpy(wrq_packet + sizeof(header), pFilename, filenamelen);
        memcpy(wrq_packet + sizeof(header) + filenamelen, mode, strlen(mode) + 1);

        int bytes = udpSocketClient->writeDatagram(wrq_packet,packetsize,tftpServer,serverPort);
        qDebug()<<"WRQ : "<<bytes<<"had been sent!";

        if(NULL != wrq_packet)
        {
            free(wrq_packet);
            wrq_packet = NULL;
        }
    }

    //open localfile and send them to server
    {
        sFile = new QFile(put_filePath);
        if(!sFile->open(QIODevice::ReadOnly))
        {
            qDebug("Can not open the file !");
            return;
        }
        send_file_size = sFile->size();
        qDebug()<<"WRQ : the size of open file : "<<sFile->size();
    }
}

void Tftp::sendDataAckMsg(struct TFTPData *pData, QHostAddress sender, quint16 senderPort)
{
    struct TFTPACK ack;
    ack.header.opcode = qToBigEndian<short>(OPCODE_ACK);
    ack.block = pData->block;

    int ack_packet_size = sizeof(ack);
    char *ack_packet = (char *)malloc(ack_packet_size);
    memcpy(ack_packet, &ack, ack_packet_size);
    int bytes = udpSocketClient->writeDatagram(ack_packet,ack_packet_size,sender,senderPort);
    qDebug("ACK : %d bytes had been sent!\n", bytes);
    if(NULL != ack_packet)
    {
        free(ack_packet);
        ack_packet = NULL;
    }
}

void Tftp::sendDataMsg(short blockno, QHostAddress sender, quint16 senderPort)
{
    char *temp_buff = (char *)malloc(BLOCKSIZE);
    int data_size = sFile->read(temp_buff, BLOCKSIZE);
    int data_packet_size = 0;

    struct TFTPData send_data;
    send_data.header.opcode = qToBigEndian<short>(OPCODE_DATA);
    send_data.block = qFromBigEndian<short>(blockno);

    qDebug()<<"send_data.block :"<< send_data.block;

    if(data_size >= 0)
    {
        data_packet_size = DATA_PACKET_HEADER_LEN + data_size;
        send_data_bytes += data_size;
    }
    char *data_packet_buff = (char *)malloc(data_packet_size);

    memcpy(data_packet_buff, &send_data, DATA_PACKET_HEADER_LEN);
    memcpy(data_packet_buff + DATA_PACKET_HEADER_LEN, temp_buff, data_size);

    int bytes = udpSocketClient->writeDatagram(data_packet_buff,data_packet_size,sender,senderPort);
    qDebug()<<"WRQ : "<<bytes<<" bytes had been sent!";

    if(NULL != temp_buff)
    {
        free(temp_buff);
        temp_buff = NULL;
    }

    if(NULL != data_packet_buff)
    {
        free(data_packet_buff);
        data_packet_buff = NULL;
    }

    if(data_size >= 0 && data_size < BLOCKSIZE)
    {
        sFile->close();
        //        wrq_block_no = 0;
        //        send_data_bytes = 0;
        put_finished_flag = true;
        f_info = "Put Finished!";
        //        ui->progressBar->setFormat(f_info);
        //        ui->progressBar->setMaximum(101);
        emit progressSig(101, f_info);
    }
}

void Tftp::readPendingDatagrams()
{
    if(udpSocketClient->hasPendingDatagrams())
    {
        QHostAddress sender;
        quint16 senderPort;
        int readbytes;

        readbytes = udpSocketClient->readDatagram(recvData, sizeof(recvData), &sender, &senderPort);
        struct TFTPHeader *header = (struct TFTPHeader*) recvData;
        struct TFTPData *data = (struct TFTPData*) recvData;
        struct TFTPACK *ack = (struct TFTPACK*) recvData;

        switch(qFromBigEndian<short>(header->opcode))
        {
        case OPCODE_DATA:
            qDebug("OPCODE_DATA");

            rFile->write(data->data, readbytes - sizeof(struct TFTPHeader) - sizeof(short));
            recv_data_bytes += (readbytes - sizeof(struct TFTPHeader) - sizeof(short));
            sendDataAckMsg(data, sender, senderPort);

            info.sprintf("   Get --- Block : %d --- RecvBytes : %d    ",(qFromBigEndian<short>(data->block)), recv_data_bytes);
            //                ui->label_Info->setText(info);
            emit progressSig(recv_data_bytes, info);

            if(readbytes < 516)   //the last packet->(data < 512 || packet_len < 516)
            {
                qDebug("File Transfer Completed");
                rFile->close();
                recv_data_bytes = 0;
                f_info = "Get Finished!";
                //                    ui->progressBar->setFormat(f_info);
                //                    ui->progressBar->setMaximum(101);
                emit progressSig(101, f_info);
            }

            break;

        case OPCODE_ACK:
            // qDebug("OPCODE_ACK");

            //construct the data and send them to the tftp server
            //struct TFTPACK *ack = (struct TFTPACK*) recvData;

            if(false == put_finished_flag)
            {
                wrq_block_no = qFromBigEndian<short>(ack->block) + 1;
                sendDataMsg(wrq_block_no, sender, senderPort);
                info.sprintf("---Put --- Block : %d --- SendBytes : %d ---",wrq_block_no, send_data_bytes);
                //                    ui->label_Info->setText(info);
                //                    ui->progressBar->setValue(((float)send_data_bytes/(float)send_file_size)*100 + 1);
                float f = ((float)send_data_bytes/(float)send_file_size)*100 + 1;
                emit progressSig(f, info);
            }
            else
            {
                wrq_block_no = 0;
                send_data_bytes = 0;
            }

            break;

        case OPCODE_ERR:
            //                ui->label_Info->setText("OPCODE ERROR! Please check you filename!");
            break;

        default:
            qDebug("OPCODE_UNKNOWN");
            //                ui->label_Info->setText("OPCODE UNKNOWN!");
            break;
        }
    }
    memset(recvData, 0, sizeof(recvData));
}

/**
 * @brief 文件上传
 * @param file 文件名，带路径 如："E:/ippdu_temhum.bin"
 * @param ip
 * @return true
 */
bool Tftp::upload(const QString &file, const QString &ip, int port, int sec)
{
    serverPort = port;
    tftpServer.setAddress(ip);

    put_filePath = file;
    put_fileInfo = QFileInfo(put_filePath);
    put_fileName = put_fileInfo.fileName();
    fileName = put_fileName.toLatin1().data();

    startDown();
    sendWriteReqMsg(fileName);

//    qDebug()<<"tftp Server Ip : "<< ip;
//    qDebug()<<"tftp Server Port : "<< serverPort;
//    qDebug()<<"filename :"<< file << put_fileName;

    for(int i=0; i<sec; ++i) {
        if(put_finished_flag || !isRun) break;
        sleep(1);
    }

    return put_finished_flag;
}

bool Tftp::getload(const QString &file, const QString &ip, int port)
{
//    get_filePath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),"/home",
//                                             QFileDialog::ShowDirsOnly
//                                             |QFileDialog::DontResolveSymlinks);

    fileName = file.toLatin1().data();
    serverPort = port;
    tftpServer.setAddress(ip);
    sendReadReqMsg(fileName);

    return true;
}
