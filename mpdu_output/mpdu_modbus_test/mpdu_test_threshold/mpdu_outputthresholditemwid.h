#ifndef MPDU_OUTPUTTHRESHOLDITEMWID_H
#define MPDU_OUTPUTTHRESHOLDITEMWID_H

#include <QWidget>

namespace Ui {
class MPDU_OutputThresholdItemWid;
}

class MPDU_OutputThresholdItemWid : public QWidget
{
    Q_OBJECT

public:
    explicit MPDU_OutputThresholdItemWid(QWidget *parent = 0);
    ~MPDU_OutputThresholdItemWid();

    void init(int id, int mode);
    void setSelect(bool checked);
    void setValue(double value);

    bool select();
    int status();

private slots:
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MPDU_OutputThresholdItemWid *ui;
};

#endif // MPDU_OUTPUTTHRESHOLDITEMWID_H
