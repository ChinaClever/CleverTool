/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AutoTestTool/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[326];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 11), // "timeoutDone"
QT_MOC_LITERAL(4, 48, 23), // "on_pushButton_4_clicked"
QT_MOC_LITERAL(5, 72, 23), // "on_pushButton_5_clicked"
QT_MOC_LITERAL(6, 96, 23), // "on_pushButton_9_clicked"
QT_MOC_LITERAL(7, 120, 24), // "on_pushButton_10_clicked"
QT_MOC_LITERAL(8, 145, 11), // "readUdpData"
QT_MOC_LITERAL(9, 157, 11), // "QByteArray&"
QT_MOC_LITERAL(10, 169, 5), // "array"
QT_MOC_LITERAL(11, 175, 13), // "QHostAddress*"
QT_MOC_LITERAL(12, 189, 4), // "addr"
QT_MOC_LITERAL(13, 194, 30), // "on_pushButton_continue_clicked"
QT_MOC_LITERAL(14, 225, 26), // "on_pushButton_stop_clicked"
QT_MOC_LITERAL(15, 252, 24), // "on_pushButton_11_clicked"
QT_MOC_LITERAL(16, 277, 24), // "on_pushButton_12_clicked"
QT_MOC_LITERAL(17, 302, 23) // "on_pushButton_7_clicked"

    },
    "MainWindow\0on_pushButton_3_clicked\0\0"
    "timeoutDone\0on_pushButton_4_clicked\0"
    "on_pushButton_5_clicked\0on_pushButton_9_clicked\0"
    "on_pushButton_10_clicked\0readUdpData\0"
    "QByteArray&\0array\0QHostAddress*\0addr\0"
    "on_pushButton_continue_clicked\0"
    "on_pushButton_stop_clicked\0"
    "on_pushButton_11_clicked\0"
    "on_pushButton_12_clicked\0"
    "on_pushButton_7_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    2,   80,    2, 0x08 /* Private */,
      13,    0,   85,    2, 0x08 /* Private */,
      14,    0,   86,    2, 0x08 /* Private */,
      15,    0,   87,    2, 0x08 /* Private */,
      16,    0,   88,    2, 0x08 /* Private */,
      17,    0,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_3_clicked(); break;
        case 1: _t->timeoutDone(); break;
        case 2: _t->on_pushButton_4_clicked(); break;
        case 3: _t->on_pushButton_5_clicked(); break;
        case 4: _t->on_pushButton_9_clicked(); break;
        case 5: _t->on_pushButton_10_clicked(); break;
        case 6: _t->readUdpData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QHostAddress*(*)>(_a[2]))); break;
        case 7: _t->on_pushButton_continue_clicked(); break;
        case 8: _t->on_pushButton_stop_clicked(); break;
        case 9: _t->on_pushButton_11_clicked(); break;
        case 10: _t->on_pushButton_12_clicked(); break;
        case 11: _t->on_pushButton_7_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
