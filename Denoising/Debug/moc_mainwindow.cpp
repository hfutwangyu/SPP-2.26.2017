/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[26];
    char stringdata0[538];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "ImportMesh"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "ExportMesh"
QT_MOC_LITERAL(4, 34, 16), // "TransToNoisyMesh"
QT_MOC_LITERAL(5, 51, 19), // "TransToOriginalMesh"
QT_MOC_LITERAL(6, 71, 19), // "TransToDenoisedMesh"
QT_MOC_LITERAL(7, 91, 9), // "ClearMesh"
QT_MOC_LITERAL(8, 101, 15), // "ApplyAlgorithms"
QT_MOC_LITERAL(9, 117, 14), // "algorithm_name"
QT_MOC_LITERAL(10, 132, 15), // "ShowNoiseWidget"
QT_MOC_LITERAL(11, 148, 32), // "ShowBilateralMeshDenoisingWidget"
QT_MOC_LITERAL(12, 181, 22), // "ShowScanPathPlanWidget"
QT_MOC_LITERAL(13, 204, 19), // "ShowSimpleSPPWidget"
QT_MOC_LITERAL(14, 224, 52), // "ShowNonIterativeFeaturePreser..."
QT_MOC_LITERAL(15, 277, 56), // "ShowFastAndEffectiveFeaturePr..."
QT_MOC_LITERAL(16, 334, 50), // "ShowBilateralNormalFilteringF..."
QT_MOC_LITERAL(17, 385, 40), // "ShowMeshDenoisingViaL0Minimiz..."
QT_MOC_LITERAL(18, 426, 35), // "ShowGuidedMeshNormalFiltering..."
QT_MOC_LITERAL(19, 462, 18), // "setActionAndWidget"
QT_MOC_LITERAL(20, 481, 6), // "value1"
QT_MOC_LITERAL(21, 488, 6), // "value2"
QT_MOC_LITERAL(22, 495, 14), // "needToUpdateGL"
QT_MOC_LITERAL(23, 510, 5), // "value"
QT_MOC_LITERAL(24, 516, 15), // "ShowSingleLayer"
QT_MOC_LITERAL(25, 532, 5) // "About"

    },
    "MainWindow\0ImportMesh\0\0ExportMesh\0"
    "TransToNoisyMesh\0TransToOriginalMesh\0"
    "TransToDenoisedMesh\0ClearMesh\0"
    "ApplyAlgorithms\0algorithm_name\0"
    "ShowNoiseWidget\0ShowBilateralMeshDenoisingWidget\0"
    "ShowScanPathPlanWidget\0ShowSimpleSPPWidget\0"
    "ShowNonIterativeFeaturePreservingMeshFilteringWidget\0"
    "ShowFastAndEffectiveFeaturePreservingMeshDenoisingWidget\0"
    "ShowBilateralNormalFilteringForMeshDenoisingWidget\0"
    "ShowMeshDenoisingViaL0MinimizationWidget\0"
    "ShowGuidedMeshNormalFilteringWidget\0"
    "setActionAndWidget\0value1\0value2\0"
    "needToUpdateGL\0value\0ShowSingleLayer\0"
    "About"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x08 /* Private */,
       3,    0,  115,    2, 0x08 /* Private */,
       4,    0,  116,    2, 0x08 /* Private */,
       5,    0,  117,    2, 0x08 /* Private */,
       6,    0,  118,    2, 0x08 /* Private */,
       7,    0,  119,    2, 0x08 /* Private */,
       8,    1,  120,    2, 0x08 /* Private */,
      10,    0,  123,    2, 0x08 /* Private */,
      11,    0,  124,    2, 0x08 /* Private */,
      12,    0,  125,    2, 0x08 /* Private */,
      13,    0,  126,    2, 0x08 /* Private */,
      14,    0,  127,    2, 0x08 /* Private */,
      15,    0,  128,    2, 0x08 /* Private */,
      16,    0,  129,    2, 0x08 /* Private */,
      17,    0,  130,    2, 0x08 /* Private */,
      18,    0,  131,    2, 0x08 /* Private */,
      19,    2,  132,    2, 0x08 /* Private */,
      22,    1,  137,    2, 0x08 /* Private */,
      24,    0,  140,    2, 0x08 /* Private */,
      25,    0,  141,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   20,   21,
    QMetaType::Void, QMetaType::Bool,   23,
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
        case 0: _t->ImportMesh(); break;
        case 1: _t->ExportMesh(); break;
        case 2: _t->TransToNoisyMesh(); break;
        case 3: _t->TransToOriginalMesh(); break;
        case 4: _t->TransToDenoisedMesh(); break;
        case 5: _t->ClearMesh(); break;
        case 6: _t->ApplyAlgorithms((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->ShowNoiseWidget(); break;
        case 8: _t->ShowBilateralMeshDenoisingWidget(); break;
        case 9: _t->ShowScanPathPlanWidget(); break;
        case 10: _t->ShowSimpleSPPWidget(); break;
        case 11: _t->ShowNonIterativeFeaturePreservingMeshFilteringWidget(); break;
        case 12: _t->ShowFastAndEffectiveFeaturePreservingMeshDenoisingWidget(); break;
        case 13: _t->ShowBilateralNormalFilteringForMeshDenoisingWidget(); break;
        case 14: _t->ShowMeshDenoisingViaL0MinimizationWidget(); break;
        case 15: _t->ShowGuidedMeshNormalFilteringWidget(); break;
        case 16: _t->setActionAndWidget((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 17: _t->needToUpdateGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->ShowSingleLayer(); break;
        case 19: _t->About(); break;
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
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
