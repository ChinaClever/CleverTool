#include "sshthread.h"

SshThread::SshThread()
{
    mflag = false;
}

void SshThread::init(QString ip ,QString count , QString password ,int timeout,bool zpduOrRpdu)
{
    mIP = ip;
    mCount = count;
    mPassword = password;
    mTimeout = timeout;
    mZpduOrRpdu = zpduOrRpdu;
    mtimes = mErr = 0;
}

QString SshThread::readChannel(QSshManager &ssh, int id)
{
    std::string str = ssh.read(id);
    qDebug()<< str.c_str();
    emit cmdSig(str.c_str());

    return str.c_str();
}

QString SshThread::sshWrite(QSshManager &ssh, int id, const QString &cmd)
{
    ssh.write(id, cmd.toLatin1().constData());
    return readChannel(ssh, id);
}

void SshThread::run()
{
    mflag = true;
    while(mflag)
    {
        mtimes ++;

        QSshManager ssh;

        QString str = tr("start connect...");
        QString cmd("");
        emit cmdSig(str);
        if (!ssh.connect(mIP.toLatin1().constData())){
            str = "connect err";
            qDebug()<<str;
            emit cmdSig(str);
            return;
        }

        str = tr("start login...");
        emit cmdSig(str);
        if (!ssh.login(mCount.toLatin1().constData(), mPassword.toLatin1().constData())){
            str = QString("login err");
            qDebug()<<str;
            emit cmdSig(str);
            return;
        }

        str = tr("start channel...");
        emit cmdSig(str);

        int id = ssh.createChannel("ubuntu1");
        if (id == -1) {
            str = QString("channel err");
            qDebug()<<str;
            emit cmdSig(str);
            return;
        }

        readChannel(ssh, id);
        if(!mZpduOrRpdu)//RPDU
        {
            sshWrite(ssh, id, mCount);
            sshWrite(ssh, id, mPassword);
        }

        if(mZpduOrRpdu)  cmd = "status 0 output";
        else  cmd = "status 0 all";

        sshWrite(ssh, id, "off 0 all");
        sleep(mTimeout);
        QString retStr = sshWrite(ssh, id, cmd);
        if(retStr.contains("ON")) mErr++;

        sshWrite(ssh, id, "on 0 all");
        sleep(mTimeout);
        retStr = sshWrite(ssh, id, cmd);
        if(retStr.contains("OFF")) mErr++;

        sshWrite(ssh, id, "quit");
        sshWrite(ssh, id, "y");

        str = QString("============= times:%1    err: %2  ============").arg(mtimes).arg(mErr);
        emit lableSig(mtimes,mErr);
        qDebug()<<str; emit cmdSig(str);
    }
}

void SshThread::stopThread()
{
    mflag = false;
}
