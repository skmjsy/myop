/****************************************************************************
** Meta object code from reading C++ file 'onroad.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "onroad.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'onroad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OnroadAlerts_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OnroadAlerts_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OnroadAlerts_t qt_meta_stringdata_OnroadAlerts = {
    {
QT_MOC_LITERAL(0, 0, 12) // "OnroadAlerts"

    },
    "OnroadAlerts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OnroadAlerts[] = {

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

void OnroadAlerts::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject OnroadAlerts::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_OnroadAlerts.data,
    qt_meta_data_OnroadAlerts,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OnroadAlerts::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnroadAlerts::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OnroadAlerts.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OnroadAlerts::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_NvgWindow_t {
    QByteArrayData data[29];
    char stringdata0[316];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NvgWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NvgWindow_t qt_meta_stringdata_NvgWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "NvgWindow"
QT_MOC_LITERAL(1, 10, 12), // "valueChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "speed"
QT_MOC_LITERAL(4, 30, 9), // "speedUnit"
QT_MOC_LITERAL(5, 40, 8), // "maxSpeed"
QT_MOC_LITERAL(6, 49, 13), // "is_cruise_set"
QT_MOC_LITERAL(7, 63, 10), // "engageable"
QT_MOC_LITERAL(8, 74, 8), // "dmActive"
QT_MOC_LITERAL(9, 83, 6), // "hideDM"
QT_MOC_LITERAL(10, 90, 6), // "status"
QT_MOC_LITERAL(11, 97, 12), // "showHowAlert"
QT_MOC_LITERAL(12, 110, 10), // "howWarning"
QT_MOC_LITERAL(13, 121, 7), // "showVTC"
QT_MOC_LITERAL(14, 129, 8), // "vtcSpeed"
QT_MOC_LITERAL(15, 138, 8), // "vtcColor"
QT_MOC_LITERAL(16, 147, 11), // "showDebugUI"
QT_MOC_LITERAL(17, 159, 8), // "roadName"
QT_MOC_LITERAL(18, 168, 14), // "showSpeedLimit"
QT_MOC_LITERAL(19, 183, 10), // "speedLimit"
QT_MOC_LITERAL(20, 194, 10), // "slcSubText"
QT_MOC_LITERAL(21, 205, 14), // "slcSubTextSize"
QT_MOC_LITERAL(22, 220, 20), // "mapSourcedSpeedLimit"
QT_MOC_LITERAL(23, 241, 9), // "slcActive"
QT_MOC_LITERAL(24, 251, 18), // "showTurnSpeedLimit"
QT_MOC_LITERAL(25, 270, 14), // "turnSpeedLimit"
QT_MOC_LITERAL(26, 285, 10), // "tscSubText"
QT_MOC_LITERAL(27, 296, 9), // "tscActive"
QT_MOC_LITERAL(28, 306, 9) // "curveSign"

    },
    "NvgWindow\0valueChanged\0\0speed\0speedUnit\0"
    "maxSpeed\0is_cruise_set\0engageable\0"
    "dmActive\0hideDM\0status\0showHowAlert\0"
    "howWarning\0showVTC\0vtcSpeed\0vtcColor\0"
    "showDebugUI\0roadName\0showSpeedLimit\0"
    "speedLimit\0slcSubText\0slcSubTextSize\0"
    "mapSourcedSpeedLimit\0slcActive\0"
    "showTurnSpeedLimit\0turnSpeedLimit\0"
    "tscSubText\0tscActive\0curveSign"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NvgWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      26,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495003,
       4, QMetaType::QString, 0x00495003,
       5, QMetaType::QString, 0x00495003,
       6, QMetaType::Bool, 0x00495003,
       7, QMetaType::Bool, 0x00495003,
       8, QMetaType::Bool, 0x00495003,
       9, QMetaType::Bool, 0x00495003,
      10, QMetaType::Int, 0x00495003,
      11, QMetaType::Bool, 0x00495003,
      12, QMetaType::Bool, 0x00495003,
      13, QMetaType::Bool, 0x00495003,
      14, QMetaType::QString, 0x00495003,
      15, QMetaType::QColor, 0x00495003,
      16, QMetaType::Bool, 0x00495003,
      17, QMetaType::QString, 0x00495003,
      18, QMetaType::Bool, 0x00495003,
      19, QMetaType::QString, 0x00495003,
      20, QMetaType::QString, 0x00495003,
      21, QMetaType::Float, 0x00495003,
      22, QMetaType::Bool, 0x00495003,
      23, QMetaType::Bool, 0x00495003,
      24, QMetaType::Bool, 0x00495003,
      25, QMetaType::QString, 0x00495003,
      26, QMetaType::QString, 0x00495003,
      27, QMetaType::Bool, 0x00495003,
      28, QMetaType::Int, 0x00495003,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

void NvgWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NvgWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NvgWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NvgWindow::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<NvgWindow *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->speed; break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->speedUnit; break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->maxSpeed; break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->is_cruise_set; break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->engageable; break;
        case 5: *reinterpret_cast< bool*>(_v) = _t->dmActive; break;
        case 6: *reinterpret_cast< bool*>(_v) = _t->hideDM; break;
        case 7: *reinterpret_cast< int*>(_v) = _t->status; break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->showHowAlert; break;
        case 9: *reinterpret_cast< bool*>(_v) = _t->howWarning; break;
        case 10: *reinterpret_cast< bool*>(_v) = _t->showVTC; break;
        case 11: *reinterpret_cast< QString*>(_v) = _t->vtcSpeed; break;
        case 12: *reinterpret_cast< QColor*>(_v) = _t->vtcColor; break;
        case 13: *reinterpret_cast< bool*>(_v) = _t->showDebugUI; break;
        case 14: *reinterpret_cast< QString*>(_v) = _t->roadName; break;
        case 15: *reinterpret_cast< bool*>(_v) = _t->showSpeedLimit; break;
        case 16: *reinterpret_cast< QString*>(_v) = _t->speedLimit; break;
        case 17: *reinterpret_cast< QString*>(_v) = _t->slcSubText; break;
        case 18: *reinterpret_cast< float*>(_v) = _t->slcSubTextSize; break;
        case 19: *reinterpret_cast< bool*>(_v) = _t->mapSourcedSpeedLimit; break;
        case 20: *reinterpret_cast< bool*>(_v) = _t->slcActive; break;
        case 21: *reinterpret_cast< bool*>(_v) = _t->showTurnSpeedLimit; break;
        case 22: *reinterpret_cast< QString*>(_v) = _t->turnSpeedLimit; break;
        case 23: *reinterpret_cast< QString*>(_v) = _t->tscSubText; break;
        case 24: *reinterpret_cast< bool*>(_v) = _t->tscActive; break;
        case 25: *reinterpret_cast< int*>(_v) = _t->curveSign; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<NvgWindow *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->speed != *reinterpret_cast< QString*>(_v)) {
                _t->speed = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 1:
            if (_t->speedUnit != *reinterpret_cast< QString*>(_v)) {
                _t->speedUnit = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 2:
            if (_t->maxSpeed != *reinterpret_cast< QString*>(_v)) {
                _t->maxSpeed = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 3:
            if (_t->is_cruise_set != *reinterpret_cast< bool*>(_v)) {
                _t->is_cruise_set = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 4:
            if (_t->engageable != *reinterpret_cast< bool*>(_v)) {
                _t->engageable = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 5:
            if (_t->dmActive != *reinterpret_cast< bool*>(_v)) {
                _t->dmActive = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 6:
            if (_t->hideDM != *reinterpret_cast< bool*>(_v)) {
                _t->hideDM = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 7:
            if (_t->status != *reinterpret_cast< int*>(_v)) {
                _t->status = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 8:
            if (_t->showHowAlert != *reinterpret_cast< bool*>(_v)) {
                _t->showHowAlert = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 9:
            if (_t->howWarning != *reinterpret_cast< bool*>(_v)) {
                _t->howWarning = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 10:
            if (_t->showVTC != *reinterpret_cast< bool*>(_v)) {
                _t->showVTC = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 11:
            if (_t->vtcSpeed != *reinterpret_cast< QString*>(_v)) {
                _t->vtcSpeed = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 12:
            if (_t->vtcColor != *reinterpret_cast< QColor*>(_v)) {
                _t->vtcColor = *reinterpret_cast< QColor*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 13:
            if (_t->showDebugUI != *reinterpret_cast< bool*>(_v)) {
                _t->showDebugUI = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 14:
            if (_t->roadName != *reinterpret_cast< QString*>(_v)) {
                _t->roadName = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 15:
            if (_t->showSpeedLimit != *reinterpret_cast< bool*>(_v)) {
                _t->showSpeedLimit = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 16:
            if (_t->speedLimit != *reinterpret_cast< QString*>(_v)) {
                _t->speedLimit = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 17:
            if (_t->slcSubText != *reinterpret_cast< QString*>(_v)) {
                _t->slcSubText = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 18:
            if (_t->slcSubTextSize != *reinterpret_cast< float*>(_v)) {
                _t->slcSubTextSize = *reinterpret_cast< float*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 19:
            if (_t->mapSourcedSpeedLimit != *reinterpret_cast< bool*>(_v)) {
                _t->mapSourcedSpeedLimit = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 20:
            if (_t->slcActive != *reinterpret_cast< bool*>(_v)) {
                _t->slcActive = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 21:
            if (_t->showTurnSpeedLimit != *reinterpret_cast< bool*>(_v)) {
                _t->showTurnSpeedLimit = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 22:
            if (_t->turnSpeedLimit != *reinterpret_cast< QString*>(_v)) {
                _t->turnSpeedLimit = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 23:
            if (_t->tscSubText != *reinterpret_cast< QString*>(_v)) {
                _t->tscSubText = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 24:
            if (_t->tscActive != *reinterpret_cast< bool*>(_v)) {
                _t->tscActive = *reinterpret_cast< bool*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        case 25:
            if (_t->curveSign != *reinterpret_cast< int*>(_v)) {
                _t->curveSign = *reinterpret_cast< int*>(_v);
                Q_EMIT _t->valueChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject NvgWindow::staticMetaObject = { {
    &CameraViewWidget::staticMetaObject,
    qt_meta_stringdata_NvgWindow.data,
    qt_meta_data_NvgWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NvgWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NvgWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NvgWindow.stringdata0))
        return static_cast<void*>(this);
    return CameraViewWidget::qt_metacast(_clname);
}

int NvgWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CameraViewWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 26;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 26;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void NvgWindow::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_OnroadWindow_t {
    QByteArrayData data[8];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OnroadWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OnroadWindow_t qt_meta_stringdata_OnroadWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "OnroadWindow"
QT_MOC_LITERAL(1, 13, 23), // "offroadTransitionSignal"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 7), // "offroad"
QT_MOC_LITERAL(4, 46, 17), // "offroadTransition"
QT_MOC_LITERAL(5, 64, 11), // "updateState"
QT_MOC_LITERAL(6, 76, 7), // "UIState"
QT_MOC_LITERAL(7, 84, 1) // "s"

    },
    "OnroadWindow\0offroadTransitionSignal\0"
    "\0offroad\0offroadTransition\0updateState\0"
    "UIState\0s"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OnroadWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x08 /* Private */,
       5,    1,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void OnroadWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OnroadWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->offroadTransitionSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->offroadTransition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updateState((*reinterpret_cast< const UIState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OnroadWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OnroadWindow::offroadTransitionSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OnroadWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_OnroadWindow.data,
    qt_meta_data_OnroadWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OnroadWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnroadWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OnroadWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OnroadWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void OnroadWindow::offroadTransitionSignal(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
