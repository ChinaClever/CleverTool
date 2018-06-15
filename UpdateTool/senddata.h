#ifndef SENDDATA_H
#define SENDDATA_H

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include "common/inofmanaging.h"
#include "seriorport/myseriorport.h"

class SendData : public QThread
{
    Q_OBJECT
public:
    SendData(MySeriorport *Port);
    init(QString fileName, QString currectPort, int);
signals:
    void sendOk(int value);
    void sendProgress(QString progress);

public slots:

private:
    void run();


    int getPacketNum(int bytes);
    bool responseSendFile(int num);

    QString mfileName;
    QString mCurrentPort;
    MySeriorport *myPort;
    int mAddr;
};

#endif // SENDDATA_H
