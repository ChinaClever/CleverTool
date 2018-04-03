/**
 * 思路：
 * 1、保存activeCode到HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode中
 * 2、程序每次打开时，先读取 注册表HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode，
 * 3、如果为空，则提示需要激活，如果存在，进行下一步：
 * 4、获取机器信息getMachineInfo(),计算calMachineCode，
 * 5、计算激活码calActiveCode与activeCode进行对比，一致说明已激活，否则提示激活。
 * 6、有效期限制：首次激活时，validRecord到HKEY_CURRENT_USER\\Software\\qCloudSecChk\\validRecordTime
 * 7、程序每次打开，确认已激活后，打开上述位置，读取第一次激活时间，
 * 8、与当前时间对比，根据激活码匹配情况（120，180，360，730），选择对应时间阈，提示软件是否过期，是否需要重新注册。
 * 9、另外不能单纯依赖注册表，还应加上一些隐藏文件的判断等
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "softwareregiste.h"

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <stdio.h>
#include <QUuid>
#include <tchar.h>
#include <QFile>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>
#include <QApplication>

#define SOFT_REG_KEY "HKEY_CURRENT_USER\\Software\\qCleverManager"

SoftwareRegiste::SoftwareRegiste()
{

}

/**
 * @brief 查看激活状态
 * @return  -1 未注册 -2 激活码错误  0激活码正确
 */
int SoftwareRegiste::activationJudge()
{
    int ret = -1;
    QString activeCode = readRegistry();
    if(!activeCode.isEmpty()) {
        QString machineCode = this->getMachineCode(0); //获取机器码
        QString activeCodeCal= this->calActiveCode(machineCode); // 获取激活码
        if(activeCode.trimmed() == activeCodeCal){
            ret = 0;
        } else {
            ret = -2;
        }
    }
    return ret;
}

/**
 * @brief 写注册表表
 * @param activeCode 注册码
 * @return
 */
void SoftwareRegiste::writeRegistry(const QString &activeCode)
{
    if(!activeCode.isEmpty()) {
        QSettings *reg = new QSettings(SOFT_REG_KEY,QSettings::NativeFormat);
        reg->setValue("activeCode",activeCode);
        delete reg;
    }
}

/**
 * @brief 读取注册码
 * @return
 */
QString SoftwareRegiste::readRegistry()
{
    QSettings *reg = new QSettings(SOFT_REG_KEY,QSettings::NativeFormat);
    QString activeCode = reg->value("activeCode").toString();
    delete reg;

    return activeCode;
}

/**
 * @brief 通过WMI接口获取硬件信息，但这些信息有些未必是唯一的（不同机器的数据可能类似)
 * @param type
 *      1.当前原生网卡地址：
 *      2.硬盘序列号
 *      3.获取主板序列号
 *      4.处理器ID)
 *      5.BIOS序列号
 *      6.主板型号
 * @return
 */
QString SoftwareRegiste::getWMIHWInfo(int type)
{
    QString hwInfo=("");
    QStringList sqlCmd;
    sqlCmd.clear();
    sqlCmd<<("SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))");
    //注意qt调用 wmi时，对查询语句要求很严格，所以 like之类的句子务必精确才能有结果出来
    sqlCmd<<("SELECT PNPDeviceID  FROM  Win32_DiskDrive WHERE( PNPDeviceID IS NOT NULL) AND (MediaType LIKE 'Fixed%')");
    sqlCmd<<("SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)");
    sqlCmd<<("SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)");

    QStringList columnName;
    columnName.clear();
    columnName<<("MACAddress");
    columnName<<("PNPDeviceID");
    columnName<<("SerialNumber");
    columnName<<("ProcessorId");
    columnName<<("SerialNumber");
    columnName<<("Product");

    QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",QString("."),QString("root\\cimv2"));
    QString query=("");
    if(type<sqlCmd.size())
        query=sqlCmd.at(type);

    QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
    QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
    //需要 include windows.h
    IEnumVARIANT* enumInterface = 0;
    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
    enumInterface->Reset();

    //qDebug()<<"number is "<<QString::number(objInterList->dynamicCall("Count").toInt());
    for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++)
    {
        //qDebug()<<__FILE__<<__LINE__;
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
        {
            //qDebug()<<__FILE__<<__LINE__;
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            //qDebug()<<__FILE__<<__LINE__;
            if(item){
                if(type<columnName.size()){
                    QString str = columnName.at(type);
                    QByteArray datagram(str.toLatin1()); //Or
                    const char* tempConstChar = datagram.data();
                    //qDebug()<<"the query is "<<query<<" and cn is "<<tempConstChar;
                    hwInfo=item->dynamicCall(tempConstChar).toString();
                }
                //qDebug() <<" string is "<<hwInfo;
            }else{
                qDebug() <<" item is null";
            }
        }else{
            qDebug() <<" item is false";
        }
    }
    //this->getCPUManID();
    return hwInfo;
}

/**
 * 读取硬盘的逻辑序列号，注意这不是硬盘的硬序列号，并不具有唯一性。
 * @return
 */
QString SoftwareRegiste::getHDLogicalID()
{
    DWORD VolumeSerialNumber;
    GetVolumeInformation(L"C:\\",NULL,0,&VolumeSerialNumber,NULL,NULL,NULL,0);
    return QString::number(VolumeSerialNumber,16).toUpper();
}

/**
 * @brief 通过调用汇编语言获取CPU制造商信息
 * @return
 */
QString SoftwareRegiste::getCPUManID()
{
    //用来存储信息
    DWORD deax;
    DWORD debx;
    DWORD decx;
    DWORD dedx;

    char ID[64];//存储制造商信息
    memset(ID,0,sizeof(ID));//先清空数组 ID
    __asm
    {
        mov eax,0
                cpuid
                mov deax,eax
                mov debx,ebx
                mov decx,ecx
                mov dedx,edx
    }

    memcpy(ID+0,&debx,4);//制造商信息前四个字符复制到数组
    memcpy(ID+4,&dedx,4);//中间四个
    memcpy(ID+8,&decx,4);//最后四个
    //如果返回 char * ,会出现乱码;故返回 string 值
    //qDebug()<<"manufacture id is "<<QString::fromLocal8Bit(ID);

    return QString::fromLocal8Bit(ID);
}

/**
 * @brief 使用qt 原生库获取网卡地址
 * @return
 */
QString SoftwareRegiste::getMac()
{
    QString macAddress=("");
    QList<QNetworkAddressEntry> lclInfAE;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface iface, list)
    {
        //保证获取的是本地IP地址，不是虚拟机，隧道 之类的网络地址
        //以下这句可优化
        if(!(iface.humanReadableName().contains("VMware",Qt::CaseInsensitive))&&!
                (iface.humanReadableName().contains("Tunnel",Qt::CaseInsensitive))&&!
                (iface.humanReadableName().contains("Tunneling",Qt::CaseInsensitive))&&!
                (iface.humanReadableName().contains("Loopback",Qt::CaseInsensitive))&&!
                (iface.humanReadableName().contains("Pseudo",Qt::CaseInsensitive)))
        {
//            qDebug()<<"caught iface name is "<<iface.humanReadableName();
            if(iface.hardwareAddress()!=("")){
                macAddress=iface.hardwareAddress().toUpper();
//                qDebug()<<"hdmac is "<<macAddress;
            }

        }
    }

    return macAddress;
}

/**
 * @brief 另一种获取CPU OEM的方法
 * @return
 */
QString SoftwareRegiste::getCPUID1()
{
    char    OEMString[13];
    QString result=("");
    int    iEAXValue,iEBXValue,iECXValue,iEDXValue;
    _asm
    {
        mov     eax,0
                cpuid
                mov     DWORD     PTR     OEMString,ebx
                mov     DWORD     PTR     OEMString+4,edx
                mov     DWORD     PTR     OEMString+8,ecx
                mov     BYTE     PTR     OEMString+12,0
    }

    // SetDlgItemText(IDC_STATIC1,OEMString);  //CPU 供应商 名称
    // qDebug()<<"manufacture id is "<<QString(OEMString);
    _asm
    {
        mov     eax,1
                cpuid
                mov     iEAXValue,eax
                mov     iEBXValue,ebx
                mov     iECXValue,ecx
                mov     iEDXValue,edx
    }

    int iCPUFamily=(0xf00 & iEAXValue)>>8;
    char Family[10]={0};
    _itoa_s(iCPUFamily,Family,10);
    //SetDlgItemText(IDC_STATIC2,Family);    //CPU系列
    //qDebug()<<"cpu family is "<<QString(Family);

    _asm
    {
        mov     eax,2
                CPUID
    }

    char szCPUID[129]={NULL};
    char szTmp[33]={NULL};
    unsigned long s1 = 0,s2=0;

    _asm
    {
        mov     eax,01h
                xor     edx,edx
                cpuid
                mov     s1,edx
                mov     s2,eax
    }
    //大写16 进制，宽度占8 个位置，右对齐
    sprintf_s(szTmp, "%08X%08X", s1, s2);
    strcpy_s(szCPUID, szTmp);
    _asm
    {
        mov     eax,03h
                xor     ecx,ecx
                xor     edx,edx
                cpuid
                mov     s1,edx
                mov     s2,ecx
    }
    sprintf_s(szTmp, "%08X%08X", s1, s2);
    strcat_s(szCPUID, szTmp);
    //SetDlgItemText(IDC_STATIC3,szCPUID);   //CPUID号
    //qDebug()<<"cpuid2 is "<<QString(szCPUID);
    result = QString(szCPUID).toUpper();

    return result;
}

QString SoftwareRegiste::getCPUID2(){
    DWORD   dwId1, dwId2, dwId3, dwId4;
    char   szCompany[13];
    PCHAR   pCompany = szCompany;
    szCompany[12]=0;

    _asm
    {
        pushfd
                pushad
                //取得CPU的ID号
                mov   eax,1   //功能号
                _emit   0x0f
                _emit   0xa2
                mov   dwId1,eax
                mov   dwId2,ebx
                mov   dwId3,ecx
                mov   dwId4,edx

                //取得CPU的制造公司名称
                mov   edi,pCompany   //功能号
                mov   eax,0
                _emit   0x0f
                _emit   0xa2
                mov   eax,ebx
                stosd
                mov   eax,edx
                stosd
                mov   eax,ecx
                stosd
                popad
                popfd
    }

    DWORD dwResult = 0;
    DWORD dwTemp1 = dwId1 << 12;
    DWORD dwTemp2 = dwId2 << 8 ;
    DWORD dwTemp3 = dwId3 << 4;

    QString res=QObject::tr("splitted string is %1_%2_%3_%4").arg(QString::number(dwTemp1,16)).arg(QString::number(dwTemp2,16)).arg(QString::number(dwTemp3,16)).arg(QString::number(dwId4,16));
    dwResult = dwTemp1 + dwTemp2 + dwTemp3 + dwId4;
    QString result=QString::number(dwResult,16).toUpper();
    //qDebug()<<"the result is "<<result;
    QString cpy=QString::fromLocal8Bit(szCompany);

    return result;
}

/**
 * @brief 综合以后上几个功能，获取机器码的函数：
 * @param type
 * @return
 */
QString SoftwareRegiste::getMachineCode(int type){

    QString machineInfo=("");
    machineInfo.append(this->getCPUID1());
    machineInfo.append(("@"));
    machineInfo.append(this->getCPUID2());
    machineInfo.append(("@"));

    for(int i=0;i<6;i++){
        machineInfo.append(this->getWMIHWInfo(i));
        machineInfo.append(("@"));
    }

    machineInfo.append(this->getHDLogicalID());
    machineInfo.append(("@"));
    machineInfo.append(this->getMac());

    QCryptographicHash sha1(QCryptographicHash::Sha1);
    QByteArray datagram(machineInfo.toLatin1());
    const char* tempConstChar = datagram.data();
    sha1.addData(tempConstChar);
    QString  machineCode=sha1.result().toHex();

    if(type==1){
        QClipboard *board = QApplication::clipboard();
        board->setText(machineCode);
        // QMessageBox::information (NULL,QObject::("机器码"),QObject::("机器码已经复制到粘贴板上,\r\n请发给软件供应商以获取激活码"));
    }

    return machineCode;
}


QString SoftwareRegiste::calActiveCode(QString machineCode)
{
    QCryptographicHash sha1(QCryptographicHash::Sha1); // MD4加密
    QByteArray datagram(machineCode.toLatin1());
    const char* tempConstChar = datagram.data();
    sha1.addData(tempConstChar);
    QString  activeCode=sha1.result().toHex();

    QClipboard *board = QApplication::clipboard();
    board->setText(activeCode);

    return activeCode;
}
