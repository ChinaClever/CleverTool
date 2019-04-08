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

protected:
    bool checkInput();
    void setenabled(bool e);

private slots:
    void timeoutDone(void);
    void on_ipRadio_clicked(bool checked);
    void on_rzRadio_clicked(bool checked);
    void on_okBtn_clicked();

private:
    Ui::DevSelectWid *ui;
    sDataPacket *mData;
    QTimer *timer;
    int mCount;
};

#endif // DEVSELECTWID_H
