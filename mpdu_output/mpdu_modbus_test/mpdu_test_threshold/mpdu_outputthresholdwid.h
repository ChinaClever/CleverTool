#ifndef MPDU_OUTPUTTHRESHOLDWID_H
#define MPDU_OUTPUTTHRESHOLDWID_H

#include "mpdu_outputthresholditemwid.h"
#include "serialportwid.h"
#include "modubs_rtu/si_rtuthread.h"

namespace Ui {
class MPDU_OutputThresholdWid;
}

class MPDU_OutputThresholdWid : public QWidget
{
    Q_OBJECT

public:
    explicit MPDU_OutputThresholdWid(QWidget *parent = 0);
    ~MPDU_OutputThresholdWid();

    void initwid(int mode, SerialPort *serial);

protected slots:
   void updateTextSlot(QString str);

private slots:
    void on_checkBox_clicked(bool checked);
    void on_pushButton_clicked();
    void on_curDoubleSpinBox_valueChanged(double arg1);

private:
    Ui::MPDU_OutputThresholdWid *ui;
    SI_RtuThread *mRtu;
    int mReg;

    MPDU_OutputThresholdItemWid *mWid[24];
};

#endif // MPDU_OUTPUTTHRESHOLDWID_H
