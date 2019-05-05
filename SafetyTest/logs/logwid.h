#ifndef LOGWID_H
#define LOGWID_H

#include "logcomwid.h"

class LogWid : public LogComWid
{
    Q_OBJECT
public:
    explicit LogWid(QWidget *parent = nullptr);

protected:
    void initTable();
    BasicSql *getDb();
};

#endif // LOGWID_H
