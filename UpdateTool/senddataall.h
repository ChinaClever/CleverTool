#ifndef SENDDATAALL_H
#define SENDDATAALL_H

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include "common/inofmanaging.h"
#include "seriorport/myseriorport.h"

class SendDataAll : public QThread
{
    Q_OBJECT
public:
    SendDataAll(MySeriorport *Port);
    init(QString fileName, QString currectPort, int , int);
signals:
    void sendOk(int value);
    void sendProgress(QString progress);
    void sendProgress2(QString progress);
public slots:

private:
    void run();

    int getPacketNum(int bytes);
    bool responseSendFile(int num);

    bool sendUpdateCmd(int add);
    bool responseUpdate();

    QString mfileName;
    QString mCurrentPort;
    MySeriorport *myPort;
    int mMin, mMax;
};

#endif // SENDDATAALL_H
