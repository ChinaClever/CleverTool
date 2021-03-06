#ifndef OUTPUTWID_H
#define OUTPUTWID_H

#include <QWidget>

namespace Ui {
class OutputWid;
}

class OutputWid : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWid(QWidget *parent = 0);
    ~OutputWid();

    void init(int id);
    void setSelect(bool checked);
    void setOpen(bool checked);
    void setClose(bool checked);

    bool select();
    int status();

private:
    Ui::OutputWid *ui;
};

#endif // OUTPUTWID_H
