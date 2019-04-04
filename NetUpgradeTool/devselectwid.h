#ifndef DEVSELECTWID_H
#define DEVSELECTWID_H

#include <QWidget>
#include "datapacket.h"

namespace Ui {
class DevSelectWid;
}

class DevSelectWid : public QWidget
{
    Q_OBJECT

public:
    explicit DevSelectWid(QWidget *parent = 0);
    ~DevSelectWid();

private slots:
    void timeoutDone(void);
    void on_ipRadio_clicked(bool checked);
    void on_rzRadio_clicked(bool checked);

private:
    Ui::DevSelectWid *ui;
    sDataPacket *mData;
    QTimer *timer;
};

#endif // DEVSELECTWID_H
