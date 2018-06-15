#include "json.h"

json::json(QString name )
{
    fileName = name;
}

/**
 * @brief 打开json文件，将array写入
 * @param array
 */
void json::writeFile(QByteArray &array)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly ))
    {
        qDebug() <<"文件打开成功！";
        QTextStream out(&file);
        out << array <<endl;
        file.close();
    }
    else
        qDebug() << "文件打开失败！";
}

void json::writeJson(data_json *jsonPacket,const jsonRelated &data)
{  
    // 构建 Json 数组 - Version
    QJsonArray versionArray;
    versionArray.append(4.8);
    versionArray.append(5.2);
    versionArray.append(5.7);

    // 构建 Json 对象 - Page
    QJsonObject pageObject;
    pageObject.insert("Home", "https://www.qt.io/");
    pageObject.insert("Download", "https://www.qt.io/download/");
    pageObject.insert("Developers", "https://www.qt.io/developers/");

    //    QJsonObject map_uut_info;  被测对象基本信息
    QJsonObject map_uut_info;
    map_uut_info.insert("work_order",    jsonPacket->uut_info.work_order);
    map_uut_info.insert("uut_type",      jsonPacket->uut_info.uut_type);
    map_uut_info.insert("uut_name",      jsonPacket->uut_info.uut_name);
    map_uut_info.insert("uut_code",      jsonPacket->uut_info.uut_code);
    map_uut_info.insert("serial_number", jsonPacket->uut_info.serial_number);
    map_uut_info.insert("supplier",      jsonPacket->uut_info.supplier);
    map_uut_info.insert("date_code",     jsonPacket->uut_info.date_code);
    map_uut_info.insert("lot_code",      jsonPacket->uut_info.lot_code);
    map_uut_info.insert("mould",         jsonPacket->uut_info.mould);
    map_uut_info.insert("cavity",        jsonPacket->uut_info.cavity);
    map_uut_info.insert("colour",         jsonPacket->uut_info.color);

    //    str_map.insert("uut_info",map_uut_info);

    //    QJsonObject ate_info_Object; 测试设备信息
    QJsonObject map_ate_info;
    map_ate_info.insert("ate_name",      jsonPacket->ate_info.ate_name);
    map_ate_info.insert("computer_name", jsonPacket->ate_info.computer_name);
    map_ate_info.insert("fixuer_id",     jsonPacket->ate_info.fixuer_id);

    //    QJsonObject program_info_Object; 测试程序信息
    QJsonObject map_program_info;
    map_program_info.insert("program_name",  jsonPacket->program_info.program_name);
    map_program_info.insert("program_ver",   jsonPacket->program_info.program_ver);

    //    QJsonObject map_uut_result; 物料测试结果
    QString isPass = "failed";
    if(data.phase == 1){
        if(data.result[0] && data.result[1]) isPass = "passed";
    }else{
        if(data.result[0] && data.result[1] && data.result[2] &&
           data.result[3] && data.result[4] && data.result[5]) isPass = "passed";
    }

    QJsonObject map_uut_result;
    map_uut_result.insert("operatorx",          jsonPacket->uut_result.operatorx);
    map_uut_result.insert("operation_sequence", jsonPacket->uut_result.operation_sequence);
    map_uut_result.insert("site_code",          jsonPacket->uut_result.site_code);
    map_uut_result.insert("start_time",         data.startTime);
    map_uut_result.insert("stop_time",          data.endTime);
    map_uut_result.insert("test_result",        isPass);

    QJsonObject json;
  //  json.insert("factory",    "CMFJ");
  //  json.insert("line",       "line01");
    json.insert("factory",      jsonPacket->factory);
    json.insert("line",         jsonPacket->line);
    json.insert("uut_info",     QJsonValue(map_uut_info) );
    json.insert("ate_info",     QJsonValue(map_ate_info) );
    json.insert("program_info", QJsonValue(map_program_info) );
    json.insert("uut_result",   QJsonValue(map_uut_result) );

    /*----------------------------------------------------------------------------------------------------------------*/
    // 项1
    QJsonArray test_item_list_array;

    if(data.phase == 1){ //单相
        for(int i = 0 ; i < 2 ; i++)
        {
            QJsonObject object;
            setSubItem(object,data, i, data.phase);
            test_item_list_array.append(QJsonValue(object));
        }

    }else{ //三相
        for(int i = 0 ; i < 6 ; i++)
        {
            QJsonObject object;
            setSubItem(object,data, i, data.phase);
            test_item_list_array.append(QJsonValue(object));
        }
    }

    json.insert("test_item_list",   QJsonValue(test_item_list_array));
    /*------------------------------------------------------------------------------------------------------------------*/
    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(); //compact写入json文档不做切割
    writeFile(byteArray);

}


/**
 * @brief 创建电流子项
 * @param map
 */
void json::setSubItem(QJsonObject &map , const jsonRelated &data, int i, char phase)
{
    QString item_name;
    QString limit, testValue;
    QString isPass = "failed";

    if(phase == 1){ //单相
        switch (i){
            case 0 : item_name = "L1_Vol"; break;
            case 1 : item_name = "L1_Cur"; break;
        }
        if(i < 1){
            limit  = QString::number( data.predictVol);  //电压 -- 上下限
            testValue = QString::number( data.vol[i]);   //电压 -- 测试值
        }else{
            limit  = QString::number( data.predictCur);  //电流 -- 上下限
            testValue = QString::number( data.cur[i-1]);  //电流 -- 测试值
        }
        if(data.result[i]) isPass = "passed";

    }else{ //三相
        switch (i){
            case 0 : item_name = "L1_Vol"; break;
            case 1 : item_name = "L2_Vol"; break;
            case 2 : item_name = "L3_Vol"; break;
            case 3 : item_name = "L1_Cur"; break;
            case 4 : item_name = "L2_Cur"; break;
            case 5 : item_name = "L3_Cur"; break;
        }

        if(i < 3){
            limit  = QString::number( data.predictVol);  //电压 -- 上下限
            testValue = QString::number( data.vol[i]);   //电压 -- 测试值
        }else{
            limit  = QString::number( data.predictCur);  //电流 -- 上下限
            testValue = QString::number( data.cur[i-3]);  //电流 -- 测试值
        }
        if(data.result[i]) isPass = "passed";
    }
            //QString("L%1 相电压不符预期，当前电压：%2").arg(i+1).arg(packet->vol[i]);
    map.insert("sub_item_name", item_name);
    map.insert("start_time", data.startTime);
    map.insert("stop_time", data.endTime);
    map.insert("test_result", isPass); //结果 passed | failed
    map.insert("result_desc", "6.57653E-06"); //结果描述
    map.insert("value_flag", "Y"); //值类型标志位 Y | N
    map.insert("lower_limit", limit); //下限
    map.insert("upper_limit", limit); //上限
    map.insert("test_value", testValue); //测试值

    //预留字段
    map.insert("segment_1", "segment_1");
    map.insert("segment_2", "segment_2");
    map.insert("segment_3", "segment_3");
    map.insert("segment_4", "segment_4");
    map.insert("segment_5", "segment_5");
    map.insert("segment_6", "segment_6");
}

void json::volItem(QVariantMap &map)
{

}


/**
 * @brief 打开json文件，读取array
 * @param array
 */
void json::readFile(QByteArray &array)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly ))
    {
        qDebug() <<"文件打开成功！";
        QTextStream in(&file);
        array = in.readAll().toLatin1();
        file.close();
    }
    else
        qDebug() << "文件打开失败！";
}

void json::readJson(data_json *jsonPacket)
{
       QByteArray byte_array;
       readFile(byte_array);

       QJsonDocument jsonDocument = QJsonDocument::fromJson(byte_array);
       QJsonObject jsonObject = jsonDocument.object();
       QJsonObject uut_info = jsonObject.take("uut_info").toObject();
       QJsonObject ate_info = jsonObject.take("ate_info").toObject();
       QJsonObject program_info = jsonObject.take("program_info").toObject();
       QJsonObject uut_result = jsonObject.take("uut_result").toObject();

       //工厂 + 加工线体
       strcpy(jsonPacket->factory,jsonObject.value("factory").toString().toLatin1().data());
       strcpy(jsonPacket->line,jsonObject.value("line").toString().toLatin1().data());
       qDebug() << "-------------[TOW]-------------------------";
       qDebug() << jsonPacket->factory;
       qDebug() << jsonPacket->line;

       //uut_info_mJson
       strcpy(jsonPacket->uut_info.work_order,      uut_info.value( "work_order"  ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.uut_type,        uut_info.value( "uut_type"    ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.uut_name,        uut_info.value( "uut_name"    ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.uut_code,        uut_info.value( "uut_code"    ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.serial_number,   uut_info.value("serial_number").toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.supplier,        uut_info.value( "supplier"    ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.date_code,       uut_info.value( "date_code"   ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.lot_code,        uut_info.value( "lot_code"    ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.mould,           uut_info.value( "mould"       ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.cavity,          uut_info.value( "cavity"      ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_info.color,           uut_info.value( "colour"       ).toString().toLatin1().data());

       qDebug() << "-------------[uut_info]-------------------------";
       qDebug() << jsonPacket->uut_info.work_order;
       qDebug() << jsonPacket->uut_info.uut_type;
       qDebug() << jsonPacket->uut_info.uut_name;
       qDebug() << jsonPacket->uut_info.uut_code;
       qDebug() << jsonPacket->uut_info.serial_number;
       qDebug() << jsonPacket->uut_info.supplier;
       qDebug() << jsonPacket->uut_info.date_code;
       qDebug() << jsonPacket->uut_info.lot_code;
       qDebug() << jsonPacket->uut_info.mould;
       qDebug() << jsonPacket->uut_info.cavity;
       qDebug() << jsonPacket->uut_info.color;

       //ate_info_mJson
       strcpy(jsonPacket->ate_info.ate_name,        ate_info.value( "ate_name"     ).toString().toLatin1().data());
       strcpy(jsonPacket->ate_info.computer_name,   ate_info.value( "computer_name").toString().toLatin1().data());
       strcpy(jsonPacket->ate_info.fixuer_id,       ate_info.value( "fixuer_id"    ).toString().toLatin1().data());

       qDebug() << "-------------[ate_info]-------------------------";
       qDebug() << jsonPacket->ate_info.ate_name;
       qDebug() << jsonPacket->ate_info.computer_name;
       qDebug() << jsonPacket->ate_info.fixuer_id;

       //program_info_mJson
       strcpy(jsonPacket->program_info.program_name,program_info.value( "program_name" ).toString().toLatin1().data());
       strcpy(jsonPacket->program_info.program_ver, program_info.value( "program_ver"  ).toString().toLatin1().data());

       qDebug() << "-------------[program_info]-------------------------";
       qDebug() << jsonPacket->program_info.program_name;
       qDebug() << jsonPacket->program_info.program_ver;

       //uut_result_mJson
       strcpy(jsonPacket->uut_result.operatorx,             uut_result.value( "operatorx" ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_result.operation_sequence,    uut_result.value( "operation_sequence" ).toString().toLatin1().data());
       strcpy(jsonPacket->uut_result.site_code,             uut_result.value( "site_code" ).toString().toLatin1().data());

       qDebug() << "-------------[uut_result]-------------------------";
       qDebug() << jsonPacket->uut_result.operatorx;
       qDebug() << jsonPacket->uut_result.operation_sequence;
       qDebug() << jsonPacket->uut_result.site_code;
}

