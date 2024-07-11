/****************************************************************************
** Meta object code from reading C++ file 'friendlist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../clientdayAll/friendlist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friendlist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FriendList_t {
    QByteArrayData data[7];
    char stringdata[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendList_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendList_t qt_meta_stringdata_FriendList = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FriendList"
QT_MOC_LITERAL(1, 11, 31), // "on_listWidget_itemDoubleClicked"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 61, 4), // "item"
QT_MOC_LITERAL(5, 66, 26), // "on_sigUserDialogClose_slot"
QT_MOC_LITERAL(6, 93, 8) // "UserData"

    },
    "FriendList\0on_listWidget_itemDoubleClicked\0"
    "\0QListWidgetItem*\0item\0"
    "on_sigUserDialogClose_slot\0UserData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       5,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    2,

       0        // eod
};

void FriendList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FriendList *_t = static_cast<FriendList *>(_o);
        switch (_id) {
        case 0: _t->on_listWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->on_sigUserDialogClose_slot((*reinterpret_cast< UserData(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FriendList::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FriendList.data,
      qt_meta_data_FriendList,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FriendList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendList::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FriendList.stringdata))
        return static_cast<void*>(const_cast< FriendList*>(this));
    return QWidget::qt_metacast(_clname);
}

int FriendList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
