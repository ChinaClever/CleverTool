#ifndef JSON_H
#define JSON_H

#include <QTextStream>
#include <QFile>
#include <QVariantMap>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDataStream>
#include <QIODevice>
#include "datadefine.h"





class json
{
public:
    json(QString name = "D:/save.json");
    void writeJson(data_json *jsonPacket, const jsonRelated &data);

    void readJson(data_json *jsonPacket);

private:
    QString fileName;
    //写
    void writeFile(QByteArray &array);
    void setSubItem(QJsonObject &map, const jsonRelated &data, int i, char phase);
    void volItem(QVariantMap &map);

    //读
    void readFile(QByteArray &array);


};
//extern data_json  *getJsonText();
//extern data_json *getDataJsonPacket();

#endif // JSON_H
