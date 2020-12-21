#ifndef INOFMANAGING_H
#define INOFMANAGING_H

#include <QByteArray>
#include <QDebug>
#include "common.h"

extern void send_to_packet(uchar addr,QByteArray &array);
extern void text_send_packet(uchar addr ,char *data,QByteArray &array);

extern void text_send_packet(uchar addr ,char *data,QByteArray &array, int len);
extern void text_change_send_packet(uchar addr,uchar funCode,ushort reg,ushort content, QByteArray &array);

#endif // INOFMANAGING_H
