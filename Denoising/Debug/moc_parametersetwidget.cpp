/****************************************************************************
** Meta object code from reading C++ file 'parametersetwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../parametersetwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parametersetwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ParameterWidget_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParameterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParameterWidget_t qt_meta_stringdata_ParameterWidget = {
    {
QT_MOC_LITERAL(0, 0, 15) // "ParameterWidget"

    },
    "ParameterWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParameterWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ParameterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ParameterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ParameterWidget.data,
      qt_meta_data_ParameterWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ParameterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParameterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ParameterWidget.stringdata0))
        return static_cast<void*>(const_cast< ParameterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ParameterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ParameterSetWidget_t {
    QByteArrayData data[9];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParameterSetWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParameterSetWidget_t qt_meta_stringdata_ParameterSetWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ParameterSetWidget"
QT_MOC_LITERAL(1, 19, 12), // "ReadyToApply"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 16), // "_algorithms_name"
QT_MOC_LITERAL(4, 50, 16), // "TransToNoisyMesh"
QT_MOC_LITERAL(5, 67, 19), // "TransToOriginalMesh"
QT_MOC_LITERAL(6, 87, 19), // "TransToDenoisedMesh"
QT_MOC_LITERAL(7, 107, 14), // "OnDefaultClick"
QT_MOC_LITERAL(8, 122, 12) // "OnApplyClick"

    },
    "ParameterSetWidget\0ReadyToApply\0\0"
    "_algorithms_name\0TransToNoisyMesh\0"
    "TransToOriginalMesh\0TransToDenoisedMesh\0"
    "OnDefaultClick\0OnApplyClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParameterSetWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,
       5,    0,   48,    2, 0x06 /* Public */,
       6,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   50,    2, 0x0a /* Public */,
       8,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ParameterSetWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParameterSetWidget *_t = static_cast<ParameterSetWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ReadyToApply((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->TransToNoisyMesh(); break;
        case 2: _t->TransToOriginalMesh(); break;
        case 3: _t->TransToDenoisedMesh(); break;
        case 4: _t->OnDefaultClick(); break;
        case 5: _t->OnApplyClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ParameterSetWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ParameterSetWidget::ReadyToApply)) {
                *result = 0;
            }
        }
        {
            typedef void (ParameterSetWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ParameterSetWidget::TransToNoisyMesh)) {
                *result = 1;
            }
        }
        {
            typedef void (ParameterSetWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ParameterSetWidget::TransToOriginalMesh)) {
                *result = 2;
            }
        }
        {
            typedef void (ParameterSetWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ParameterSetWidget::TransToDenoisedMesh)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ParameterSetWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_ParameterSetWidget.data,
      qt_meta_data_ParameterSetWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ParameterSetWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParameterSetWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ParameterSetWidget.stringdata0))
        return static_cast<void*>(const_cast< ParameterSetWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int ParameterSetWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ParameterSetWidget::ReadyToApply(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ParameterSetWidget::TransToNoisyMesh()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ParameterSetWidget::TransToOriginalMesh()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ParameterSetWidget::TransToDenoisedMesh()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
