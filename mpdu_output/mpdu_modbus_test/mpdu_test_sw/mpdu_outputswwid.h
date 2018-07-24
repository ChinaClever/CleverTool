#ifndef MPDU_OUTPUTSWWID_H
#define MPDU_OUTPUTSWWID_H

#include <QWidget>
#include "mpdu_outputswitemwid.h"
#include "serialportwid.h"
#include "modubs_rtu/si_rtuthread.h"

namespace Ui {
class MPDU_OutputSwWid;
}

class MPDU_OutputSwWid : public QWidget
{
    Q_OBJECT

public:
    explicit MPDU_OutputSwWid(QWidget *parent = 0);
    ~MPDU_OutputSwWid();


protected:
    void initwid();

protected slots:
   void updateTextSlot(QString str);

private slots:
    void on_checkBox_clicked(bool checked);
    void on_pushButton_clicked();
    void on_openRadio_clicked(bool checked);

    void on_closeRadio_clicked(bool checked);


private:
    Ui::MPDU_OutputSwWid *ui;
    SerialPortWid *mSerialPortWid;
    SI_RtuThread *mRtu;
    MPDU_OutputSwItemWid *mWid[24];
};

#endif // MPDU_OUTPUTSWWID_H
