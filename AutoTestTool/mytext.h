#ifndef MYTEXT_H
#define MYTEXT_H

#include <QDialog>
#include "json/json.h"
#include "datadefine.h"

namespace Ui {
class MyText;
}

class MyText : public QDialog
{
    Q_OBJECT

public:
    explicit MyText(QWidget *parent = 0);
    ~MyText();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::MyText *ui;

private:
    void updateJson();
    void initData();
    void initBackground();

    QString mFileName;

    int nameValue;
};

extern const jsonRelated &get_relatedata();

#endif // MYTEXT_H
