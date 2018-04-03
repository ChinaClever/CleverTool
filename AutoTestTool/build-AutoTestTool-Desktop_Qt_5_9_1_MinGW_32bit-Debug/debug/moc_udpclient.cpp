/****************************************************************************
** Meta object code from reading C++ file 'udpclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AutoTestTool/udp/udpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UdpClient_t {
    QByteArrayData data[9];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UdpClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UdpClient_t qt_meta_stringdata_UdpClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UdpClient"
QT_MOC_LITERAL(1, 10, 11), // "receiveData"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "QByteArray&"
QT_MOC_LITERAL(4, 35, 5), // "array"
QT_MOC_LITERAL(5, 41, 13), // "QHostAddress*"
QT_MOC_LITERAL(6, 55, 4), // "addr"
QT_MOC_LITERAL(7, 60, 11), // "timeoutDone"
QT_MOC_LITERAL(8, 72, 8) // "readData"

    },
    "UdpClient\0receiveData\0\0QByteArray&\0"
    "array\0QHostAddress*\0addr\0timeoutDone\0"
    "readData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UdpClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   34,    2, 0x0a /* Public */,
       8,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UdpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UdpClient *_t = static_cast<UdpClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receiveData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QHostAddress*(*)>(_a[2]))); break;
        case 1: _t->timeoutDone(); break;
        case 2: _t->readData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UdpClient::*_t)(QByteArray & , QHostAddress * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UdpClient::receiveData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject UdpClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UdpClient.data,
      qt_meta_data_UdpClient,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UdpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UdpClient.stringdata0))
        return static_cast<void*>(const_cast< UdpClient*>(this));
    return QObject::qt_metacast(_clname);
}

int UdpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UdpClient::receiveData(QByteArray & _t1, QHostAddress * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
