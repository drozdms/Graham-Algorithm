/****************************************************************************
** Meta object code from reading C++ file 'convexhullwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../convexhullwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'convexhullwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConvexHullWidget_t {
    QByteArrayData data[8];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConvexHullWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConvexHullWidget_t qt_meta_stringdata_ConvexHullWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ConvexHullWidget"
QT_MOC_LITERAL(1, 17, 14), // "generatePoints"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "quantity"
QT_MOC_LITERAL(4, 42, 11), // "buildConvex"
QT_MOC_LITERAL(5, 54, 5), // "reset"
QT_MOC_LITERAL(6, 60, 6), // "finish"
QT_MOC_LITERAL(7, 67, 16) // "changeDebugSpeed"

    },
    "ConvexHullWidget\0generatePoints\0\0"
    "quantity\0buildConvex\0reset\0finish\0"
    "changeDebugSpeed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConvexHullWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       1,    0,   47,    2, 0x2a /* Public | MethodCloned */,
       4,    0,   48,    2, 0x0a /* Public */,
       5,    0,   49,    2, 0x0a /* Public */,
       6,    0,   50,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void ConvexHullWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConvexHullWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->generatePoints((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->generatePoints(); break;
        case 2: _t->buildConvex(); break;
        case 3: _t->reset(); break;
        case 4: _t->finish(); break;
        case 5: _t->changeDebugSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConvexHullWidget::staticMetaObject = { {
    &QGLWidget::staticMetaObject,
    qt_meta_stringdata_ConvexHullWidget.data,
    qt_meta_data_ConvexHullWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConvexHullWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConvexHullWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConvexHullWidget.stringdata0))
        return static_cast<void*>(this);
    return QGLWidget::qt_metacast(_clname);
}

int ConvexHullWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
