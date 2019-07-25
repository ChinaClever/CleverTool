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
    QList<QStringList> mList;
    QStringList mHeaderList;

protected slots:
   void updateTextSlot(QString str, int color);
   void updateErrLabSlot(int count);

private slots:
   void on_pushButton_2_clicked();
   void on_pushButton_clicked();
   void on_stopBtn_clicked();
   void timeoutDone();
   void timeoutClearDone();

   void on_EditBtn_clicked();

private:
   bool readFile(QString &fn);
   void updateText(QStringList str);
   void transformer(sSetCmd& cmd);
   void openFile();
   void writeLog(QString &str);
   int analyseDevType(QString str);
   void saveTestData();
   QString bulidFileName();
   QString bulidPath();
   void bulidHead();
   void bulidTestData();

private:
    Ui::Mpdu_Test_WRThreshold *ui;
    SI_RtuThread *mRtu;
    QStringList mlist;
    QFile *mFile;
    QString mlogs;
    QTimer *m_timer;
    QTimer *m_cleartimer;
    uint64_t m_totalConut;
    uint64_t m_errConut;
    Excel_SaveThread *mSaveThread;
    QString mFileName;
};

#endif // MPDU_TEST_WRTHRESHOLD_H
