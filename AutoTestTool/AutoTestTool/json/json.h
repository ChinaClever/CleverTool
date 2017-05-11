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


#ifdef _DEBUG
#pragma comment(lib,"lib_json_d.lib")
#else
#pragma comment(lib,"lib_json.lib")
#endif


class json
{
public:
    json();
    void writeJson();

private:
    void writeFile(QByteArray &array);
    void curItem(QVariantMap &map);
    void volItem(QVariantMap &map);


};

extern data_json *getDataJsonPacket();

#endif // JSON_H
