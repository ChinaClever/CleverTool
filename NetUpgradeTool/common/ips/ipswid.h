#ifndef IPSWID_H
#define IPSWID_H

#include <QWidget>

namespace Ui {
class IpsWid;
}

class IpsWid : public QWidget
{
    Q_OBJECT

public:
    explicit IpsWid(QWidget *parent = 0);
    ~IpsWid();

protected:
    void disEnable();
    bool readFile(QString &fn, QStringList &list);
    int bulidIps(QString startIp, QString &endIp);
    int getIpInt(QString &ip);

public slots:
    void languageChanged();

private slots:
    void on_radio_1_clicked(bool checked);
    void on_radio_2_clicked(bool checked);
    void on_radio_3_clicked(bool checked);

    void on_exportBtn_clicked();
    void on_btn_1_clicked();
    void on_btn_2_clicked();

private:
    Ui::IpsWid *ui;
};

#endif // IPSWID_H
