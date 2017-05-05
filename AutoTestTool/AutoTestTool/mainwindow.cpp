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

    QTimer *timer_1 = new QTimer; //用于刷新ip，检测掉线
    connect(timer_1,SIGNAL(timeout()),this,SLOT(timeoutDoneOfRefreshIp()));
    timer_1->start(3*1000);

    //    timer->start(5*1000);
    UdpClient *client = getUdpClient();
    QHostAddress addr("192.168.1.112");
    int port_1 = 40051;
    int port_2 = 40052;
    client_1 = new TcpClient(addr,port_1,this); //发送数据
    client_2 = new TcpClient(addr,port_2,this); //接收传输文件
    //    client_1->createConnect();
    client_2->createConnect();
    isConnecting = true;
    connect(client,SIGNAL(receiveData(QByteArray&)),this,SLOT(readUdpData(QByteArray&)));

    //    json myJson;
    //    myJson.writeJson();
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
    mNum = 0;
    if(!client_1->getConnectingStatus()) //如果未连接，那么就连接
    {
        client_1->createConnect();
        sleep(100);
    }

    if(client_1->getConnectingStatus() && !(timer->isActive()))      //如果已连接，且定时器未开启，那么启动定时器发送命令
        timer->start(0.5*1000);

    ui->pushButton_3->setVisible(false);
    isStopButton = true;
    //     initGatherCmd();
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
    if(!isOut)
    {
        if(mNum < ret)
            for(int i = 1 ; i <= mPhaseNum; i++ )
                mAddr = i;
    }else if(mNum >=ret && mNum <(ret + mOutNum-1))
    {
        for(int i = 1 ; i <=mOutNum ;i++)
            mAddr = i;
    }

    if(isVol)
        predictValue = ui->lineEdit_predictvol->text().toInt();
    else
        predictValue = ui->lineEdit_predictcur->text().toInt();

    practicalValue = 0xFF;

    packet.addr = mAddr;
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

    //    initConnectingIp(); //更新连接ip
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
    qDebug()<< mNum;
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
        client_2->writeData(array);
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
    //    mJsonPacket.factory = ui->lineEdit_factory->text().toLatin1().data();
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

    json myJson;
    myJson.writeJson();  //将信息写入json文件
}

void MainWindow::readUdpData(QByteArray &array)
{
    data_packet netPacket;
    //    data_dev deviceData ;
    if(analysis_data(netPacket,array))
    {
        get_information(netPacket,&mDeviceData);
        initConnectingIp(); //如果从udp读到数据，那么更新ip
        initDeviceInfo(); // 更新设备信息
    }else
        qDebug() << "数据格式问题！";
}

void MainWindow::initConnectingIp()
{
    mConnectingNum = 3;
    UdpClient *client = getUdpClient();
    ui->lineEdit_ip->setText(client->getAddr().toString().remove(0,7));
    ui->label_connecting_status->setText(tr("连接正常"));


}

/**
 * @brief 检测设备是否掉线
 */
void MainWindow::timeoutDoneOfRefreshIp()
{
    //    qDebug() << "mConnectingNum:" << mConnectingNum;

    if(mConnectingNum)
        mConnectingNum--;
    else  //设备掉线
    {
        //清空ip及连接状态
        ui->lineEdit_ip->clear();
        ui->label_connecting_status->clear();

        // 清空设备基本信息
        ui->label_device_type->clear();
        ui->label_out_num->clear();
        ui->label_phase_num->clear();
    }
}

void MainWindow::initDeviceInfo()
{
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
}


/**
 * @brief MainWindow::继续
 */
void MainWindow::on_pushButton_continue_clicked()
{
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
    isStopButton = false;
    ui->pushButton_stop->setVisible(false);
    timer->stop();

}

/**
 * @brief 相位单选框
 */
void MainWindow::on_radioButton_3_clicked()
{
    //    if(isStopButton)
    //    ui->lineEdit_40
}

/**
 * @brief 输出位单选框
 */
void MainWindow::on_radioButton_2_clicked()
{
    //   ui->lineEdit_41
}

/**
 * @brief 上一项
 */
void MainWindow::on_pushButton_7_clicked()
{
    if(mNum)
        mNum--;
}

/**
 * @brief 下一项
 */
void MainWindow::on_pushButton_8_clicked()
{
    mNum++;
}
