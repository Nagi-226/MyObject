/****************************************************************************
** Meta object code from reading C++ file 'soketthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../serverDayAll/soketthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soketthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SoketThread_t {
    QByteArrayData data[9];
    char stringdata[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SoketThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SoketThread_t qt_meta_stringdata_SoketThread = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SoketThread"
QT_MOC_LITERAL(1, 12, 14), // "loginSucSignal"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "UserData"
QT_MOC_LITERAL(4, 37, 12), // "SoketThread*"
QT_MOC_LITERAL(5, 50, 14), // "sigUserOffLine"
QT_MOC_LITERAL(6, 65, 14), // "sigUserSendMsg"
QT_MOC_LITERAL(7, 80, 17), // "on_readyRead_slot"
QT_MOC_LITERAL(8, 98, 20) // "on_disconnected_slot"

    },
    "SoketThread\0loginSucSignal\0\0UserData\0"
    "SoketThread*\0sigUserOffLine\0sigUserSendMsg\0"
    "on_readyRead_slot\0on_disconnected_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SoketThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    1,   44,    2, 0x06 /* Public */,
       6,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   50,    2, 0x0a /* Public */,
       8,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SoketThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SoketThread *_t = static_cast<SoketThread *>(_o);
        switch (_id) {
        case 0: _t->loginSucSignal((*reinterpret_cast< UserData(*)>(_a[1])),(*reinterpret_cast< SoketThread*(*)>(_a[2]))); break;
        case 1: _t->sigUserOffLine((*reinterpret_cast< UserData(*)>(_a[1]))); break;
        case 2: _t->sigUserSendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->on_readyRead_slot(); break;
        case 4: _t->on_disconnected_slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SoketThread* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SoketThread::*_t)(UserData , SoketThread * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SoketThread::loginSucSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SoketThread::*_t)(UserData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SoketThread::sigUserOffLine)) {
                *result = 1;
            }
        }
        {
            typedef void (SoketThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SoketThread::sigUserSendMsg)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SoketThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SoketThread.data,
      qt_meta_data_SoketThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SoketThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SoketThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SoketThread.stringdata))
        return static_cast<void*>(const_cast< SoketThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SoketThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SoketThread::loginSucSignal(UserData _t1, SoketThread * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SoketThread::sigUserOffLine(UserData _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SoketThread::sigUserSendMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
