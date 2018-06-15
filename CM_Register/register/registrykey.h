#ifndef REGISTRYKEY_H
#define REGISTRYKEY_H
#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <stdio.h>
#include <QUuid>
#include <tchar.h>
#include <QFile>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>
#include <QApplication>


class RegistryKey
{
public:
    RegistryKey();

    void writeDate(const QDate &date);
    void writeCount(int count);
    void writeMachineCode(const QString &code);
    void writeActiveCode(const QString &code);

    QDate readDate();
    int readCount();
    QString readMachineCode();
    QString readActiveCode();

protected:
    QString read(const QString &key);
    void write(const QString &key,const QString &str);
};

#endif // REGISTRYKEY_H
