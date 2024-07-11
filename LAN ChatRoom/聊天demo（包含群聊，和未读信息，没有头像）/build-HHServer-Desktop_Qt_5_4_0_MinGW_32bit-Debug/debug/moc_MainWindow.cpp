/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HHServer/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "newConnection"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "qintptr"
QT_MOC_LITERAL(4, 34, 6), // "handle"
QT_MOC_LITERAL(5, 41, 12), // "loginSuccess"
QT_MOC_LITERAL(6, 54, 20), // "MsgBuilder::UserData"
QT_MOC_LITERAL(7, 75, 4), // "user"
QT_MOC_LITERAL(8, 80, 8), // "QThread*"
QT_MOC_LITERAL(9, 89, 6), // "thread"
QT_MOC_LITERAL(10, 96, 7), // "sendMsg"
QT_MOC_LITERAL(11, 104, 4), // "from"
QT_MOC_LITERAL(12, 109, 2), // "to"
QT_MOC_LITERAL(13, 112, 3), // "msg"
QT_MOC_LITERAL(14, 116, 11), // "userOffline"
QT_MOC_LITERAL(15, 128, 11), // "createGroup"
QT_MOC_LITERAL(16, 140, 4), // "name"
QT_MOC_LITERAL(17, 145, 11), // "searchGroup"
QT_MOC_LITERAL(18, 157, 9), // "joinGroup"
QT_MOC_LITERAL(19, 167, 7), // "groupId"
QT_MOC_LITERAL(20, 175, 10), // "leaveGroup"
QT_MOC_LITERAL(21, 186, 12) // "sendGroupMsg"

    },
    "MainWindow\0newConnection\0\0qintptr\0"
    "handle\0loginSuccess\0MsgBuilder::UserData\0"
    "user\0QThread*\0thread\0sendMsg\0from\0to\0"
    "msg\0userOffline\0createGroup\0name\0"
    "searchGroup\0joinGroup\0groupId\0leaveGroup\0"
    "sendGroupMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       5,    2,   62,    2, 0x08 /* Private */,
      10,    3,   67,    2, 0x08 /* Private */,
      14,    1,   74,    2, 0x08 /* Private */,
      15,    2,   77,    2, 0x08 /* Private */,
      17,    1,   82,    2, 0x08 /* Private */,
      18,    2,   85,    2, 0x08 /* Private */,
      20,    2,   90,    2, 0x08 /* Private */,
      21,    4,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6, QMetaType::QString,   11,   12,   13,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,   16,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   19,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   19,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 6, QMetaType::Int, QMetaType::QString,    9,   11,   12,   13,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->newConnection((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 1: _t->loginSuccess((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1])),(*reinterpret_cast< QThread*(*)>(_a[2]))); break;
        case 2: _t->sendMsg((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1])),(*reinterpret_cast< MsgBuilder::UserData(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->userOffline((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1]))); break;
        case 4: _t->createGroup((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QThread*(*)>(_a[2]))); break;
        case 5: _t->searchGroup((*reinterpret_cast< QThread*(*)>(_a[1]))); break;
        case 6: _t->joinGroup((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->leaveGroup((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->sendGroupMsg((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< MsgBuilder::UserData(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
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
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
