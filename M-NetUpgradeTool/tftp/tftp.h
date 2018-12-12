#ifndef TFTP_H
#define TFTP_H

#include "common\datapacket.h"
#include <QFileDialog>

class Tftp : public QObject
{
    Q_OBJECT
public:
    explicit Tftp(QObject *parent = 0);

    bool upload(const QString &file, const QString &ip);
signals:

public slots:

private:
    char cFile[512],cUrl[512];
};

#endif // TFTP_H
