#ifndef MPDU_OUTPUTSWITEMWID_H
#define MPDU_OUTPUTSWITEMWID_H

#include <QWidget>

namespace Ui {
class MPDU_OutputSwItemWid;
}

class MPDU_OutputSwItemWid : public QWidget
{
    Q_OBJECT

public:
    explicit MPDU_OutputSwItemWid(QWidget *parent = 0);
    ~MPDU_OutputSwItemWid();

    void init(int id);
    void setSelect(bool checked);
    void setOpen(bool checked);
    void setClose(bool checked);

    bool select();
    int status();

private:
    Ui::MPDU_OutputSwItemWid *ui;
};

#endif // MPDU_OUTPUTSWITEMWID_H
