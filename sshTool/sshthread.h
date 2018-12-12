#ifndef SSHTHREAD_H
#define SSHTHREAD_H
#include <QThread>
#include "qsshmanager.h"

class SshThread : public QThread
{
    Q_OBJECT
public:
    SshThread();
    void init(QString ip , QString count , QString password , int timeout, bool zpduOrRpdu);
    void stopThread();

protected:
    QString readChannel(QSshManager &ssh, int id);
    QString sshWrite(QSshManager &ssh, int id, const QString &cmd);

private:
    void run();
    bool mflag;
    uint mtimes, mErr;

    QString mIP;
    QString mCount;
    QString mPassword;
    int mTimeout;
    bool mZpduOrRpdu;

signals:
    void cmdSig(QString);
    void lableSig(int,int);

public slots:
};

#endif // SSHTHREAD_H
