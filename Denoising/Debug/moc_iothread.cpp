/****************************************************************************
** Meta object code from reading C++ file 'iothread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../iothread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iothread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ioThread_t {
    QByteArrayData data[5];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ioThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ioThread_t qt_meta_stringdata_ioThread = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ioThread"
QT_MOC_LITERAL(1, 9, 17), // "statusShowMessage"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 18), // "setActionAndWidget"
QT_MOC_LITERAL(4, 47, 14) // "needToUpdateGL"

    },
    "ioThread\0statusShowMessage\0\0"
    "setActionAndWidget\0needToUpdateGL"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ioThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    2,   32,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void ioThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ioThread *_t = static_cast<ioThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->statusShowMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setActionAndWidget((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->needToUpdateGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ioThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ioThread::statusShowMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (ioThread::*_t)(bool , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ioThread::setActionAndWidget)) {
                *result = 1;
            }
        }
        {
            typedef void (ioThread::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ioThread::needToUpdateGL)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ioThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ioThread.data,
      qt_meta_data_ioThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ioThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ioThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ioThread.stringdata0))
        return static_cast<void*>(const_cast< ioThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ioThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void ioThread::statusShowMessage(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ioThread::setActionAndWidget(bool _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ioThread::needToUpdateGL(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
