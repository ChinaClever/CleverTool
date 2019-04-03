#ifndef MPDU_TEST_WRTHRESHOLD_H
#define MPDU_TEST_WRTHRESHOLD_H

#include <QWidget>
#include "modubs_rtu/si_rtuthread.h"

namespace Ui {
class Mpdu_Test_WRThreshold;
}

class Mpdu_Test_WRThreshold : public QWidget
{
    Q_OBJECT

public:
    explicit Mpdu_Test_WRThreshold(QWidget *parent = 0);
    ~Mpdu_Test_WRThreshold();
    void initwid(SerialPort *serial);

protected slots:
   void updateTextSlot(QString str);
   void updateErrLabSlot(int count);

private slots:
   void on_pushButton_2_clicked();
   void on_pushButton_clicked();
   void on_stopBtn_clicked();

private:
   bool readFile(QString &fn);
   void updateText(QStringList str);
   void transformer(sSetCmd& cmd);

private:
    Ui::Mpdu_Test_WRThreshold *ui;
    SI_RtuThread *mRtu;
    QStringList mlist;
};

#endif // MPDU_TEST_WRTHRESHOLD_H
