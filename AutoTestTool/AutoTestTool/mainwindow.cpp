#include "mainwindow.h"
#include "ui_mainwindow.h"

static data_json mJsonPacket;

data_json *getDataJsonPacket()
{
    return &mJsonPacket;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mConnectingNum = 3;
    isStopButton = false;

    ui->widget_2->hide();
    this->setFixedHeight(360);

    timer = new QTimer;  //用于控制命令发送间隔
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutDone()));

    client_1 = new TcpClient(this);
    client_2 = new TcpClient(this);
    isConnecting = true;

    UdpClient *client = getUdpClient();
    connect(client,SIGNAL(receiveData(QByteArray&,QHostAddress*)),this,SLOT(readUdpData(QByteArray&,QHostAddress*)));

    json myJson;
    myJson.writeJson();  //将信息写入json文件
#if 1
    MySql sql;
    sql.createSqlite();
    sql.openDataBase();
    sql.createTable();

    sql.insertData("192.168.1.112","10:45");
    sql.insertData("192.168.1.113","10:46");
    sql.insertData("192.168.1.114","10:47");
    sql.insertData("192.168.1.115","10:48");
    sql.insertData("192.168.1.116","10:49");
    sql.insertData("192.168.1.117","10:40");
    sql.insertData("192.168.1.118","10:41");

    sql.searchAllDatabase();
    //    sql.searchDatabase();
    //    if(!sql.searchOneData(3))
    //        qDebug() << "数据不存在！";
    //    sql.updateDatabase();
    //    sql.delateData(11);

    //    sql.clearTable();
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief 点击之后开始循环发送命令，间隔十秒
 */
void MainWindow::on_pushButton_3_clicked()
{

    mNum = 0 ;
    mOutAddr = mPhaseAddr = 1 ;
    int time = ui->spinBox_time->value();

    if(client_1->getConnectingStatus() && !(timer->isActive()))      //如果已连接，且定时器未开启，那么启动定时器发送命令
    {
        timer->start(time*1000);

        //设置按钮状态为stop
        ui->pushButton_3->setVisible(false);
        isStopButton = true;
    }

}

/**
 * @brief 初始化采集数据的命令mGatherCmdPacket
 */
void MainWindow::initGatherCmd(data_packet &packet)
{

    quint16 predictValue ;
    quint16 practicalValue ;
    //    addr = 12;
    int ret = mPhaseNum*2;

    qDebug() << "initGatherCmd:" <<mNum ;

    if(!isOut)
    {
        if((mNum - 1) < ret)
            packet.addr = (mNum + 1)/2;

        ui->radioButton_phase->setChecked(true);   //修改checkbox状态
        ui->lineEdit_radio_phase->setText(QString::number(packet.addr,10));
    }else
    {
        if(((mNum - 1) >= ret) && ((mNum - 1) <= (ret + mOutNum-1)))
        {
            mPhaseAddr = mNum - ret;
            packet.addr = mPhaseAddr;
            //            qDebug() << mNum;
            ui->radioButton_out->setChecked(true);
            ui->lineEdit_radio_out->setText(QString::number(packet.addr));
        }
    }

    if(isVol)
        predictValue = ui->lineEdit_predictvol->text().toInt();
    else
        predictValue = ui->lineEdit_predictcur->text().toInt();

    practicalValue = 0xFF;

    //    packet.addr = mAddr;
    packet.predictValue = predictValue;
    packet.practicalValue = practicalValue;

    cmd_to_netpacket(isOut,isVol,&packet); //第一个false代表主控，第二个true 代表电压

    packet.len = 8;
    packet.XOR = 0;
    packet.XOR += packet.addr;
    packet.XOR += packet.predictValue;
    packet.XOR += packet.practicalValue;
    packet.XOR += packet.reserve;
    packet.XOR += packet.result;

}

void MainWindow::timeoutDone()
{   
    sentCmd();
}

/**
 * @brief M改变全局变量mNum,改变发送命令
 */
void MainWindow::sentCmd()
{
    typeSet();  //确定是主控或输出位？相位几或输出位几
    data_packet mGatherCmdPacket;  //采集命令
    initGatherCmd(mGatherCmdPacket);
    QByteArray array;
    packetToArray(mGatherCmdPacket,array);
    if(isConnecting)
        client_1->writeData(array); //40051

}

void  MainWindow::packetToArray(data_packet &packet, QByteArray &array)
{
    array.append(packet.har);
    array.append(packet.stx);
    array.append(packet.trans);
    array.append(packet.fn);

    array.append(packet.len >> 8);
    array.append(packet.len & 0xFF);

    array.append(packet.addr);

    array.append(packet.predictValue >> 8);
    array.append(packet.predictValue & 0xFF);

    array.append(packet.practicalValue >> 8);
    array.append(packet.practicalValue & 0xFF);

    array.append(packet.reserve >> 8);
    array.append(packet.reserve & 0xFF);

    array.append(packet.result);

    array.append(packet.XOR);

    array.append(packet.ED);
}

/**
 * @brief 依次循环测试，包括主控及输出位，测试完毕，定时器停止
 */
void MainWindow::typeSet()
{
    //    qDebug()<< mNum;
    int ret = mPhaseNum*2;

    if(mNum < ret)
    {
        isOut = false;
        isVol = mNum%2;
        mNum++;
    } else if(mNum >=ret && mNum <(ret + mOutNum-1))
    {
        isOut = true;
        isVol = false;
        mNum++;
    } else if(mNum >= (ret + mOutNum -1))  //超过一次测试数量
    {
        timer->stop();

        //发送完毕后，按钮恢复yuanla状态
        ui->pushButton_continue->setVisible(false);
        ui->pushButton_3->setVisible(true);
        ui->pushButton_continue->setVisible(true);

        mNum++;
    }



}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * @brief 导入
 */
void MainWindow::on_pushButton_4_clicked()
{
    int len = 1024; //文件长度
    data_packet importCmdPacket;
    filecmd_to_netpacket(false,&importCmdPacket,len);
    QByteArray array;
    packetToArray(importCmdPacket,array);
    if(isConnecting)
        client_1->writeData(array);



    int port  = 40052;
    client_2->createConnect(*mAddr,port);
    QString str = "D:/save.json";

    sleep(10);

    if(client_2->getConnectingStatus())
        client_2->sendFile(str);
    else
        qDebug() << "未连接";
}

/**
 * @brief 导出
 */
void MainWindow::on_pushButton_5_clicked()
{
    int len = 0; //文件长度
    data_packet exportCmdPacket;
    filecmd_to_netpacket(true,&exportCmdPacket,len);
    QByteArray array;
    packetToArray(exportCmdPacket,array);
    if(isConnecting)
        client_1->writeData(array);
}

/**
 * @brief 详细信息按钮
 */
void MainWindow::on_pushButton_9_clicked()
{
    if(ui->widget_2->isHidden())
    {
        ui->widget_2->show();
        this->setFixedHeight(742);
        ui->pushButton_9->setText(tr("隐藏信息"));
    }else
    {
        ui->widget_2->hide();
        this->setFixedHeight(360);
        ui->pushButton_9->setText(tr("详细信息"));
    }
}

/**
 * @brief 补充json数据
 */
void MainWindow::initJsonDataPacket()
{
    mJsonPacket.basic_info.subOutItemNum  = mOutNum;
    mJsonPacket.basic_info.subPhaseItemNum  = mPhaseNum;

    strcpy(mJsonPacket.factory , ui->lineEdit_factory->text().toLatin1().data());
    strcpy(mJsonPacket.line , ui->lineEdit_line->text().toLatin1().data());

    strcpy(mJsonPacket.uut_info.work_order , ui->lineEdit_work_order->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.uut_type , ui->lineEdit_uut_type->text().toLatin1().data());
    strcpy( mJsonPacket.uut_info.uut_name , ui->lineEdit_uut_name->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.uut_code , ui->lineEdit_uut_code->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.serial_number , ui->lineEdit_serial_number->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.supplier , ui->lineEdit_supplier ->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.date_code , ui->lineEdit_date_code->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.lot_code , ui->lineEdit_lot_code->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.mould , ui->lineEdit_mould->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.cavity , ui->lineEdit_cavity->text().toLatin1().data());
    strcpy(mJsonPacket.uut_info.color , ui->lineEdit_color->text().toLatin1().data());

    strcpy(mJsonPacket.ate_info.ate_name , ui->lineEdit_ate_name->text().toLatin1().data());
    strcpy(mJsonPacket.ate_info.computer_name , ui->lineEdit_computer_name->text().toLatin1().data());
    strcpy(mJsonPacket.ate_info.fixuer_id , ui->lineEdit_fixuer_id->text().toLatin1().data());

    strcpy(mJsonPacket.program_info.program_name , ui->lineEdit_program_name->text().toLatin1().data());
    strcpy(mJsonPacket.program_info.program_ver , ui->lineEdit_program_ver->text().toLatin1().data());

    strcpy(mJsonPacket.uut_result.operatorx , ui->lineEdit_operator->text().toLatin1().data());
    strcpy(mJsonPacket.uut_result.operation_sequence , ui->lineEdit_operation_sequence->text().toLatin1().data());
    strcpy(mJsonPacket.uut_result.site_code , ui->lineEdit_site_code->text().toLatin1().data());
}

/**
 * @brief 保存输入信息
 */
void MainWindow::on_pushButton_10_clicked()
{
    initJsonDataPacket();

    //    json myJson;
    //    myJson.writeJson();  //将信息写入json文件
}

void MainWindow::readUdpData(QByteArray &array, QHostAddress *addr)
{

    data_packet netPacket;
    if(analysis_data(netPacket,array) && !(client_1->getConnectingStatus()))  //防止client的重复连接
    {
        get_information(netPacket,&mDeviceData);

        int port = 40051;
        client_1->createConnect(*addr,port); //读取数据之后，先建立连接
        sleep(10);
        if(client_1->getConnectingStatus())
        {
            initDeviceInfo(addr); // 更新设备信息
        }

    }else
        qDebug() << "数据格式问题！";
}


/**
 * @brief 检测设备是否掉线
 */
void MainWindow::clearDeviceInfo()
{

    //清空ip及连接状态
    ui->lineEdit_ip->clear();
    ui->label_connecting_status->setText(tr("无连接"));

    // 清空设备基本信息
    ui->label_device_type->clear();
    ui->label_out_num->clear();
    ui->label_phase_num->clear();

    //按钮
    ui->pushButton_11->setEnabled(true);

}

void MainWindow::initDeviceInfo(QHostAddress *addr)
{
    //ip信息初始化
    ui->lineEdit_ip->setText(addr->toString().remove(0,7));
    ui->label_connecting_status->setText(tr("连接正常"));
    ui->pushButton_11->setEnabled(false);

    //设备类型
    QString deviceType;
    switch (mDeviceData.info.devType) {
    case 1:
        deviceType = tr("MPDU");
        break;
    case 2:
        deviceType = tr("RPDU");
        break;
    default:
        qDebug() << "设备类型error";
        break;
    }
    ui->label_device_type->setText(deviceType);
    qDebug() << "输出位：" << mDeviceData.info.outNum << "相位："  << mDeviceData.info.phaseNum;
    //输出位
    ui->label_out_num->setText(QString::number(mDeviceData.info.outNum,10));

    //相位
    ui->label_phase_num->setText(QString::number(mDeviceData.info.phaseNum,10));

    //改变发送命令中的想位数及输出位数
    mOutNum = mDeviceData.info.outNum;
    mPhaseNum = mDeviceData.info.phaseNum;

    //全局变量连接ip
    mAddr = addr;
}


/**
 * @brief MainWindow::继续
 */
void MainWindow::on_pushButton_continue_clicked()
{
    isStopButton = false;
    ui->pushButton_continue->setVisible(false);
    ui->pushButton_3->setVisible(true);
    ui->pushButton_continue->setVisible(true);
    ui->pushButton_stop->setVisible(true);
    timer->start();
}

/**
 * @brief MainWindow::暂停
 */
void MainWindow::on_pushButton_stop_clicked()
{
    //    isStopButton = false;
    ui->pushButton_stop->setVisible(false);
    timer->stop();

}

/**
 * @brief 自动搜索
 */
void MainWindow::on_pushButton_11_clicked()
{
    UdpClient *client = getUdpClient();
    client->sendHeartBeart(); //点击按钮发送心跳包
}

/**
 * @brief 断开连接
 */
void MainWindow::on_pushButton_12_clicked()
{
    client_1->breakConnect();
    clearDeviceInfo();
}


/**
 * @brief 检测是否为纯数字
 * @param str
 * @return
 */
bool MainWindow::checkIsDigit(QString &str)
{

    for(int i = 0 ; i < str.length() ; i++)
    {
        if(!isdigit(*(str.toLatin1().data() + i)))
            return false;
    }

    return true;
}

/**
 * @brief 检测lineedit输入是否合法
 * @param lineEdit
 * @param upNum -- 输出位个数，或者是输入相个数
 * @return
 */
bool MainWindow::checkRadioInputIsRight(QLineEdit *lineEdit,int &upNum)
{
    QString str = ui->lineEdit_radio_phase->text();
    if(checkIsDigit(str))
    {
        int num = lineEdit->text().toInt();
        if((num > 0) && (num <= upNum))
        {

        }else
        {
            if(num)
                QMessageBox::warning(this,tr("warning"),tr("输入超过上限，请重新输入"));
            else
                QMessageBox::warning(this,tr("warning"),tr("输入不能为0,请重新输入"));

            lineEdit->clear();
            return false;

        }
    }else
    {
        QMessageBox::warning(this,tr("warning"),tr("请输入数字"));
        lineEdit->clear();
        return false;
    }

    return true;
}

/**
 * @brief 单元测试
 */
void MainWindow::on_pushButton_7_clicked()
{
    int num = 0;

    if(ui->radioButton_phase->isChecked())  //输入相
    {
        if(!ui->lineEdit_radio_phase->text().isEmpty())  //输入不为空
        {
            if(checkRadioInputIsRight(ui->lineEdit_radio_phase,mPhaseNum))  //输入合法
            {
                num = ui->lineEdit_radio_phase->text().toInt();

                mNum = (num - 1)*2;
                sentCmd(); //电流
                sleep(10);
                sentCmd(); //电压
            }
        }else
        {
            QMessageBox::warning(this,tr("waring"),tr("请选择输入相！"));
        }

    }else  //输出位
    {
        if(!ui->lineEdit_radio_out->text().isEmpty())
        {

            if(checkRadioInputIsRight(ui->lineEdit_radio_out,mOutNum))
            {
                num = ui->lineEdit_radio_out->text().toInt();
                mNum = mPhaseNum*2 + (num - 1);
                sentCmd();
            }
        }else
        {
            QMessageBox::warning(this,tr("waring"),tr("请选择输出位！"));
        }
    }

}
