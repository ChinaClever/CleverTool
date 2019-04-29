#ifndef TGSWID_H
#define TGSWID_H

#include <QWidget>

namespace Ui {
class TgsWid;
}

class TgsWid : public QWidget
{
    Q_OBJECT

public:
    explicit TgsWid(QWidget *parent = 0);
    ~TgsWid();

private:
    Ui::TgsWid *ui;
};

#endif // TGSWID_H
