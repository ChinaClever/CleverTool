#ifndef ICOM_GLOBAL_H
#define ICOM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ICOM_LIBRARY)
#  define ICOMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICOMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ICOM_GLOBAL_H
