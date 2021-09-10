#ifndef SYSCONFIGFILE_H
#define SYSCONFIGFILE_H
#include <QString>


extern bool sys_configFile_open(void);
extern void sys_configFile_close(void);
extern  QString sys_configFile_readStr(QString, QString strGroup="System");
extern int sys_configFile_readInt(QString, QString strGroup="System");
extern double sys_configFile_readDouble(QString, QString strGroup="System");
extern void sys_configFile_write(QString, QString, QString strGroup="System");

extern void sys_configFile_writeParam(QString, QString, QString strGroup="System");

#endif // SYSCONFIGFILE_H

