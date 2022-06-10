/****************************************************************************
** Meta object code from reading C++ file 'route_engine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "route_engine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'route_engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RouteEngine_t {
    QByteArrayData data[7];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RouteEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RouteEngine_t qt_meta_stringdata_RouteEngine = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RouteEngine"
QT_MOC_LITERAL(1, 12, 11), // "timerUpdate"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 15), // "routeCalculated"
QT_MOC_LITERAL(4, 41, 15), // "QGeoRouteReply*"
QT_MOC_LITERAL(5, 57, 5), // "reply"
QT_MOC_LITERAL(6, 63, 14) // "recomputeRoute"

    },
    "RouteEngine\0timerUpdate\0\0routeCalculated\0"
    "QGeoRouteReply*\0reply\0recomputeRoute"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RouteEngine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       6,    0,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void RouteEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RouteEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timerUpdate(); break;
        case 1: _t->routeCalculated((*reinterpret_cast< QGeoRouteReply*(*)>(_a[1]))); break;
        case 2: _t->recomputeRoute(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RouteEngine::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_RouteEngine.data,
    qt_meta_data_RouteEngine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RouteEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RouteEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RouteEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RouteEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
