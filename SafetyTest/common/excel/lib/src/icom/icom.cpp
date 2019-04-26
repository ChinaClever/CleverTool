#include "icom.h"
#include <QtCore>
#include <sapi.h>

Icom::Icom()
{
    HRESULT r = OleInitialize(0);
    if (r != S_OK && r != S_FALSE)
        qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);
}
