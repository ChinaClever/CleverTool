#include "ipswid.h"
#include "ui_ipswid.h"
#include <QFileDialog>
#include "common/msgbox.h"
#include "common/datapacket.h"
#include <QHostAddress>

bool cm_isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}

IpsWid::IpsWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IpsWid)
{
    ui->setupUi(this);
    on_radio_2_clicked(true);
}

IpsWid::~IpsWid()
{
    delete ui;
}

void IpsWid::disEnable()
{
    ui->widget->setDisabled(true);
    ui->exportBtn->setDisabled(true);
    ui->ipEdit->setDisabled(true);
    ui->btn_1->setDisabled(true);
}


void IpsWid::on_radio_1_clicked(bool checked)
{
    disEnable();
    ui->exportBtn->setEnabled(checked);
}

void IpsWid::on_radio_2_clicked(bool checked)
{
    disEnable();
    ui->ipEdit->setEnabled(checked);
    ui->btn_1->setEnabled(checked);
}

void IpsWid::on_radio_3_clicked(bool checked)
{
    disEnable();
    ui->widget->setEnabled(checked);
}


bool IpsWid::readFile(QString &fn, QStringList &list)
{
    int i=0;
    QFile file(fn);
    if(file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            i++;
            QString str = QString(file.readLine()).trimmed().remove("\r\n");
            if(list.contains(str)) continue;

            bool ret = cm_isIPaddress(str);
            if(ret) {
                list  << str;
            } else {
                CriticalMsgBox bos(this, tr("第%1条，%2 不是IP地址!!").arg(i).arg(str));
                return false;
            }
        }
    }

    return true;
}

void IpsWid::on_exportBtn_clicked()
{
    QString fn = QFileDialog::getOpenFileName(0,tr("文件选择"),"/","",0);
    if (!fn.isNull()) {
        QStringList list;
        bool ret = readFile(fn, list);
        if(ret) {
            DataPacket::bulid()->data->ips = list;
        }
    }
}

void IpsWid::on_btn_1_clicked()
{
    QString ip = ui->ipEdit->text();
    bool ret = cm_isIPaddress(ip);
    if(ret) {
        QStringList list; list << ip;
        DataPacket::bulid()->data->ips = list;
    } else {
        CriticalMsgBox bos(this, tr("%1 不是IP地址!!").arg(ip));
    }
}

int IpsWid::getIpInt(QString &ip)
{
    QHostAddress sendip(ip);
    return sendip.toIPv4Address();
}

int IpsWid::bulidIps( QString startIp,  QString &endIp)
{
    QStringList list;
    QHostAddress startAddr(startIp), endAddr(endIp);
    for(uint i=startAddr.toIPv4Address(); i<=endAddr.toIPv4Address(); ++i)
    {
        list << QHostAddress(i).toString().remove("::ffff:"); // IPV4删除这个字符串
    }
    DataPacket::bulid()->data->ips = list;

    return list.size();
}

void IpsWid::on_btn_2_clicked()
{
    QString start = ui->startEdit->text();
    QString end = ui->endEdit->text();

    bool ret = cm_isIPaddress(start);
    if(ret) {
        ret = cm_isIPaddress(end);
        if(ret) {
            int rtn = bulidIps(start, end);
            if(!rtn) {
                CriticalMsgBox bos(this, tr("IP地址生成错误!!"));
            }
        } else {
            CriticalMsgBox bos(this, tr("%1 不是IP地址!!").arg(end));
        }
    } else {
        CriticalMsgBox bos(this, tr("起始IP:%1 不是IP地址!!").arg(start));
    }
}
