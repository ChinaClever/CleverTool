#include "json.h"

json::json()
{

}

/**
 * @brief 打开json文件，将array写入
 * @param array
 */
void json::writeFile(QByteArray &array)
{
    QFile file("D:/save.json");
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

void json::writeJson()
{
    //    Json::Value root;

    //       //根节点属性
    //       root["name"] = Json::Value("Tsybius");
    //       root["age"] = Json::Value(23);
    //       root["sex_is_male"] = Json::Value(true);


    data_json *jsonPacket = getDataJsonPacket();

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

    //    QJsonObject map_uut_info;
    QJsonObject map_uut_info;
    map_uut_info.insert("work_order", jsonPacket->uut_info.work_order);
    map_uut_info.insert("uut_type", jsonPacket->uut_info.uut_type);
    map_uut_info.insert("uut_name", jsonPacket->uut_info.uut_name);
    map_uut_info.insert("uut_code", jsonPacket->uut_info.uut_code);
    map_uut_info.insert("serial_number", jsonPacket->uut_info.serial_number);
    map_uut_info.insert("supplier", jsonPacket->uut_info.supplier);
    map_uut_info.insert("date_code", jsonPacket->uut_info.date_code);
    map_uut_info.insert("lot_code", jsonPacket->uut_info.lot_code);
    map_uut_info.insert("mould",jsonPacket->uut_info.mould);
    map_uut_info.insert("cavity", jsonPacket->uut_info.cavity);
    map_uut_info.insert("colour", jsonPacket->uut_info.color);

    //    str_map.insert("uut_info",map_uut_info);

    //    QJsonObject ate_info_Object;
    QJsonObject map_ate_info;
    map_ate_info.insert("ate_name", jsonPacket->ate_info.ate_name);
    map_ate_info.insert("computer_name", jsonPacket->ate_info.computer_name);
    map_ate_info.insert("fixuer_id", jsonPacket->ate_info.fixuer_id);

    //    QJsonObject program_info_Object;
    QJsonObject map_program_info;
    map_program_info.insert("program_name", jsonPacket->program_info.program_name);
    map_program_info.insert("program_ver", jsonPacket->program_info.program_ver);

    //    QJsonObject map_uut_result;
    QJsonObject map_uut_result;
    map_uut_result.insert("operator", jsonPacket->uut_result.operatorx);
    map_uut_result.insert("operation_sequence", jsonPacket->uut_result.operation_sequence);
    map_uut_result.insert("site_code", jsonPacket->uut_result.site_code);
    map_uut_result.insert("start_time", "2016-07-11 11:48:50");
    map_uut_result.insert("stop_time", "2016-07-11 11:48:50");
    map_uut_result.insert("test_result", "passed");

    //    QMap<QString, QVariant> str_map;
    //    str_map.insert("factory", jsonPacket->factory);
    //    str_map.insert("line", jsonPacket->line);
    //    str_map.insert("uut_info",map_uut_info );
    //    str_map.insert("ate_info",map_ate_info );
    //    str_map.insert("program_info",map_program_info );
    //    str_map.insert("uut_result",map_uut_result);

    QJsonObject json;
    json.insert("factory", "CMFJ");
    json.insert("line", "line01");
    json.insert("uut_info",QJsonValue(map_uut_info) );
    json.insert("ate_info",QJsonValue(map_ate_info) );
    json.insert("program_info",QJsonValue(map_program_info) );
    json.insert("uut_result",QJsonValue(map_uut_result) );
    //    json.insert("uut_result",QJsonValue(map_test_item_list) );
    //    json.insert("sub_test_item_list", QJsonValue(map_sub1_test_item_list));
    //    json.insert("", QJsonValue(map_sub2_test_item_list));
    /*----------------------------------------------------------------------------------------------------------------*/
    // 项
    QJsonObject map_test_item_list_1;
    map_test_item_list_1.insert("item_name", "P0");
    map_test_item_list_1.insert("start_time", "2016-07-11 11:48:50");
    map_test_item_list_1.insert("stop_time", "2016-07-11 11:48:50");
    map_test_item_list_1.insert("test_result", "passed");
    map_test_item_list_1.insert("result_desc", "");
    map_test_item_list_1.insert("value_flag", "N");
    map_test_item_list_1.insert("lower_limit", "");
    map_test_item_list_1.insert("upper_limit", "");
    map_test_item_list_1.insert("test_value", "");


    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(); //compact写入json文档不做切割
    writeFile(byteArray);

    //    QJsonDocument jsonDocument = QJsonDocument::fromVariant(QVariant(str_map));
    //    if (!jsonDocument.isNull()) {
    //        QByteArray json = jsonDocument.toJson();
    //        writeFile(json);
}



/**
 * @brief 创建电流子项
 * @param map
 */
void json::curItem(QVariantMap &map)
{
    map.insert("sub_item_name", "LX");
    map.insert("start_time", "2016-07-11 11:48:50");
    map.insert("stop_time", "2016-07-11 11:48:50");
    map.insert("test_result", "passed");
    map.insert("result_desc", "6.57653E-06");
    map.insert("value_flag", "Y");
    map.insert("lower_limit", "5.98E-6");
    map.insert("upper_limit", "7.02E-6");
    map.insert("test_value", "6.57653E-06");
}

void json::volItem(QVariantMap &map)
{

}
