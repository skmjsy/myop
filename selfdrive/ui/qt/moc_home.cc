/****************************************************************************
** Meta object code from reading C++ file 'home.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "home.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'home.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OffroadHome_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OffroadHome_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OffroadHome_t qt_meta_stringdata_OffroadHome = {
    {
QT_MOC_LITERAL(0, 0, 11) // "OffroadHome"

    },
    "OffroadHome"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OffroadHome[] = {

 // content:
       8,       // revision
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

void OffroadHome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject OffroadHome::staticMetaObject = { {
    &QFrame::staticMetaObject,
    qt_meta_stringdata_OffroadHome.data,
    qt_meta_data_OffroadHome,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OffroadHome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OffroadHome::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OffroadHome.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int OffroadHome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_HomeWindow_t {
    QByteArrayData data[10];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomeWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomeWindow_t qt_meta_stringdata_HomeWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "HomeWindow"
QT_MOC_LITERAL(1, 11, 12), // "openSettings"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13), // "closeSettings"
QT_MOC_LITERAL(4, 39, 23), // "offroadTransitionSignal"
QT_MOC_LITERAL(5, 63, 7), // "offroad"
QT_MOC_LITERAL(6, 71, 17), // "offroadTransition"
QT_MOC_LITERAL(7, 89, 14), // "showDriverView"
QT_MOC_LITERAL(8, 104, 4), // "show"
QT_MOC_LITERAL(9, 109, 11) // "showSidebar"

    },
    "HomeWindow\0openSettings\0\0closeSettings\0"
    "offroadTransitionSignal\0offroad\0"
    "offroadTransition\0showDriverView\0show\0"
    "showSidebar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomeWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    1,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   49,    2, 0x0a /* Public */,
       7,    1,   52,    2, 0x0a /* Public */,
       9,    1,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void HomeWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HomeWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openSettings(); break;
        case 1: _t->closeSettings(); break;
        case 2: _t->offroadTransitionSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->offroadTransition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->showDriverView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->showSidebar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HomeWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomeWindow::openSettings)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HomeWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomeWindow::closeSettings)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HomeWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomeWindow::offroadTransitionSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HomeWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_HomeWindow.data,
    qt_meta_data_HomeWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HomeWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomeWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HomeWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HomeWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void HomeWindow::openSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HomeWindow::closeSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HomeWindow::offroadTransitionSignal(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
