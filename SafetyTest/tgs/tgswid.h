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

protected:
    int workDown();
    bool checkInput();
    void updateData(int all, int ok);

private slots:
    void on_queryBtn_clicked();
    void on_trRadio_clicked(bool checked);
    void on_todayRadio_clicked(bool checked);
    void on_codeRadio_clicked(bool checked);
    void on_allRadio_clicked(bool checked);

private:
    Ui::TgsWid *ui;
};

#endif // TGSWID_H
