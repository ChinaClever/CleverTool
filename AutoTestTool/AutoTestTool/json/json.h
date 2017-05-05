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
    json();
    void writeJson();

private:
    void writeFile(QByteArray &array);


};

extern data_json *getDataJsonPacket();

#endif // JSON_H
