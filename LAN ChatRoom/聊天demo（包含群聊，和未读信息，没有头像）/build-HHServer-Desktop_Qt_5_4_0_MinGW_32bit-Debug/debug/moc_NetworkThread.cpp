/****************************************************************************
** Meta object code from reading C++ file 'NetworkThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../HHServer/NetworkThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetWorkThread_t {
    QByteArrayData data[22];
    char stringdata[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetWorkThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetWorkThread_t qt_meta_stringdata_NetWorkThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "NetWorkThread"
QT_MOC_LITERAL(1, 14, 12), // "loginSuccess"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 20), // "MsgBuilder::UserData"
QT_MOC_LITERAL(4, 49, 4), // "user"
QT_MOC_LITERAL(5, 54, 8), // "QThread*"
QT_MOC_LITERAL(6, 63, 6), // "thread"
QT_MOC_LITERAL(7, 70, 7), // "sendMsg"
QT_MOC_LITERAL(8, 78, 4), // "from"
QT_MOC_LITERAL(9, 83, 2), // "to"
QT_MOC_LITERAL(10, 86, 3), // "msg"
QT_MOC_LITERAL(11, 90, 11), // "userOffline"
QT_MOC_LITERAL(12, 102, 11), // "createGroup"
QT_MOC_LITERAL(13, 114, 4), // "name"
QT_MOC_LITERAL(14, 119, 4), // "self"
QT_MOC_LITERAL(15, 124, 11), // "searchGroup"
QT_MOC_LITERAL(16, 136, 9), // "joinGroup"
QT_MOC_LITERAL(17, 146, 7), // "groupId"
QT_MOC_LITERAL(18, 154, 10), // "leaveGroup"
QT_MOC_LITERAL(19, 165, 12), // "sendGroupMsg"
QT_MOC_LITERAL(20, 178, 9), // "readyRead"
QT_MOC_LITERAL(21, 188, 12) // "disconnected"

    },
    "NetWorkThread\0loginSuccess\0\0"
    "MsgBuilder::UserData\0user\0QThread*\0"
    "thread\0sendMsg\0from\0to\0msg\0userOffline\0"
    "createGroup\0name\0self\0searchGroup\0"
    "joinGroup\0groupId\0leaveGroup\0sendGroupMsg\0"
    "readyRead\0disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetWorkThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       7,    3,   69,    2, 0x06 /* Public */,
      11,    1,   76,    2, 0x06 /* Public */,
      12,    2,   79,    2, 0x06 /* Public */,
      15,    1,   84,    2, 0x06 /* Public */,
      16,    2,   87,    2, 0x06 /* Public */,
      18,    2,   92,    2, 0x06 /* Public */,
      19,    4,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    0,  106,    2, 0x08 /* Private */,
      21,    0,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, QMetaType::QString,    8,    9,   10,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5,   13,   14,
    QMetaType::Void, 0x80000000 | 5,   14,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,   14,   17,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,   14,   17,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 3, QMetaType::Int, QMetaType::QString,   14,    8,    9,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetWorkThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetWorkThread *_t = static_cast<NetWorkThread *>(_o);
        switch (_id) {
        case 0: _t->loginSuccess((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1])),(*reinterpret_cast< QThread*(*)>(_a[2]))); break;
        case 1: _t->sendMsg((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1])),(*reinterpret_cast< MsgBuilder::UserData(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->userOffline((*reinterpret_cast< MsgBuilder::UserData(*)>(_a[1]))); break;
        case 3: _t->createGroup((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QThread*(*)>(_a[2]))); break;
        case 4: _t->searchGroup((*reinterpret_cast< QThread*(*)>(_a[1]))); break;
        case 5: _t->joinGroup((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->leaveGroup((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->sendGroupMsg((*reinterpret_cast< QThread*(*)>(_a[1])),(*reinterpret_cast< MsgBuilder::UserData(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 8: _t->readyRead(); break;
        case 9: _t->disconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetWorkThread::*_t)(MsgBuilder::UserData , QThread * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::loginSuccess)) {
                *result = 0;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(MsgBuilder::UserData , MsgBuilder::UserData , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::sendMsg)) {
                *result = 1;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(MsgBuilder::UserData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::userOffline)) {
                *result = 2;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(QString , QThread * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::createGroup)) {
                *result = 3;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(QThread * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::searchGroup)) {
                *result = 4;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(QThread * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::joinGroup)) {
                *result = 5;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(QThread * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::leaveGroup)) {
                *result = 6;
            }
        }
        {
            typedef void (NetWorkThread::*_t)(QThread * , MsgBuilder::UserData , int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetWorkThread::sendGroupMsg)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject NetWorkThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NetWorkThread.data,
      qt_meta_data_NetWorkThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NetWorkThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetWorkThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NetWorkThread.stringdata))
        return static_cast<void*>(const_cast< NetWorkThread*>(this));
    return QThread::qt_metacast(_clname);
}

int NetWorkThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void NetWorkThread::loginSuccess(MsgBuilder::UserData _t1, QThread * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetWorkThread::sendMsg(MsgBuilder::UserData _t1, MsgBuilder::UserData _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetWorkThread::userOffline(MsgBuilder::UserData _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetWorkThread::createGroup(QString _t1, QThread * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetWorkThread::searchGroup(QThread * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetWorkThread::joinGroup(QThread * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetWorkThread::leaveGroup(QThread * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetWorkThread::sendGroupMsg(QThread * _t1, MsgBuilder::UserData _t2, int _t3, QString _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
