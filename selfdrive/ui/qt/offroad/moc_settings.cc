/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "settings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsWindow_t {
    QByteArrayData data[7];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsWindow_t qt_meta_stringdata_SettingsWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SettingsWindow"
QT_MOC_LITERAL(1, 15, 13), // "closeSettings"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 19), // "reviewTrainingGuide"
QT_MOC_LITERAL(4, 50, 14), // "showDriverView"
QT_MOC_LITERAL(5, 65, 23), // "expandToggleDescription"
QT_MOC_LITERAL(6, 89, 5) // "param"

    },
    "SettingsWindow\0closeSettings\0\0"
    "reviewTrainingGuide\0showDriverView\0"
    "expandToggleDescription\0param"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void SettingsWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeSettings(); break;
        case 1: _t->reviewTrainingGuide(); break;
        case 2: _t->showDriverView(); break;
        case 3: _t->expandToggleDescription((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingsWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsWindow::closeSettings)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingsWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsWindow::reviewTrainingGuide)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SettingsWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsWindow::showDriverView)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SettingsWindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsWindow::expandToggleDescription)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsWindow::staticMetaObject = { {
    &QFrame::staticMetaObject,
    qt_meta_stringdata_SettingsWindow.data,
    qt_meta_data_SettingsWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsWindow.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int SettingsWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SettingsWindow::closeSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SettingsWindow::reviewTrainingGuide()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SettingsWindow::showDriverView()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SettingsWindow::expandToggleDescription(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_DevicePanel_t {
    QByteArrayData data[8];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DevicePanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DevicePanel_t qt_meta_stringdata_DevicePanel = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DevicePanel"
QT_MOC_LITERAL(1, 12, 19), // "reviewTrainingGuide"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 14), // "showDriverView"
QT_MOC_LITERAL(4, 48, 13), // "closeSettings"
QT_MOC_LITERAL(5, 62, 8), // "poweroff"
QT_MOC_LITERAL(6, 71, 6), // "reboot"
QT_MOC_LITERAL(7, 78, 22) // "updateCalibDescription"

    },
    "DevicePanel\0reviewTrainingGuide\0\0"
    "showDriverView\0closeSettings\0poweroff\0"
    "reboot\0updateCalibDescription"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DevicePanel[] = {

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
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DevicePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DevicePanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reviewTrainingGuide(); break;
        case 1: _t->showDriverView(); break;
        case 2: _t->closeSettings(); break;
        case 3: _t->poweroff(); break;
        case 4: _t->reboot(); break;
        case 5: _t->updateCalibDescription(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DevicePanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevicePanel::reviewTrainingGuide)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DevicePanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevicePanel::showDriverView)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DevicePanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevicePanel::closeSettings)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DevicePanel::staticMetaObject = { {
    &ListWidget::staticMetaObject,
    qt_meta_stringdata_DevicePanel.data,
    qt_meta_data_DevicePanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DevicePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DevicePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DevicePanel.stringdata0))
        return static_cast<void*>(this);
    return ListWidget::qt_metacast(_clname);
}

int DevicePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ListWidget::qt_metacall(_c, _id, _a);
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
void DevicePanel::reviewTrainingGuide()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DevicePanel::showDriverView()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DevicePanel::closeSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_TogglesPanel_t {
    QByteArrayData data[4];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TogglesPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TogglesPanel_t qt_meta_stringdata_TogglesPanel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TogglesPanel"
QT_MOC_LITERAL(1, 13, 23), // "expandToggleDescription"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5) // "param"

    },
    "TogglesPanel\0expandToggleDescription\0"
    "\0param"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TogglesPanel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void TogglesPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TogglesPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->expandToggleDescription((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TogglesPanel::staticMetaObject = { {
    &ListWidget::staticMetaObject,
    qt_meta_stringdata_TogglesPanel.data,
    qt_meta_data_TogglesPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TogglesPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TogglesPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TogglesPanel.stringdata0))
        return static_cast<void*>(this);
    return ListWidget::qt_metacast(_clname);
}

int TogglesPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ListWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}
struct qt_meta_stringdata_SoftwarePanel_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SoftwarePanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SoftwarePanel_t qt_meta_stringdata_SoftwarePanel = {
    {
QT_MOC_LITERAL(0, 0, 13) // "SoftwarePanel"

    },
    "SoftwarePanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SoftwarePanel[] = {

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

void SoftwarePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SoftwarePanel::staticMetaObject = { {
    &ListWidget::staticMetaObject,
    qt_meta_stringdata_SoftwarePanel.data,
    qt_meta_data_SoftwarePanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SoftwarePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SoftwarePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SoftwarePanel.stringdata0))
        return static_cast<void*>(this);
    return ListWidget::qt_metacast(_clname);
}

int SoftwarePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ListWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_C2NetworkPanel_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_C2NetworkPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_C2NetworkPanel_t qt_meta_stringdata_C2NetworkPanel = {
    {
QT_MOC_LITERAL(0, 0, 14) // "C2NetworkPanel"

    },
    "C2NetworkPanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_C2NetworkPanel[] = {

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

void C2NetworkPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject C2NetworkPanel::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_C2NetworkPanel.data,
    qt_meta_data_C2NetworkPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *C2NetworkPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *C2NetworkPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_C2NetworkPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int C2NetworkPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_SelectCar_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectCar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectCar_t qt_meta_stringdata_SelectCar = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SelectCar"
QT_MOC_LITERAL(1, 10, 9), // "backPress"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11) // "selectedCar"

    },
    "SelectCar\0backPress\0\0selectedCar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectCar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SelectCar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SelectCar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backPress(); break;
        case 1: _t->selectedCar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SelectCar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectCar::backPress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SelectCar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectCar::selectedCar)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SelectCar::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SelectCar.data,
    qt_meta_data_SelectCar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SelectCar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectCar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SelectCar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SelectCar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void SelectCar::backPress()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SelectCar::selectedCar()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_CommunityPanel_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommunityPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommunityPanel_t qt_meta_stringdata_CommunityPanel = {
    {
QT_MOC_LITERAL(0, 0, 14) // "CommunityPanel"

    },
    "CommunityPanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommunityPanel[] = {

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

void CommunityPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CommunityPanel::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CommunityPanel.data,
    qt_meta_data_CommunityPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CommunityPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunityPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommunityPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CommunityPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_TuningPanel_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TuningPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TuningPanel_t qt_meta_stringdata_TuningPanel = {
    {
QT_MOC_LITERAL(0, 0, 11) // "TuningPanel"

    },
    "TuningPanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TuningPanel[] = {

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

void TuningPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TuningPanel::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TuningPanel.data,
    qt_meta_data_TuningPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TuningPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TuningPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TuningPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TuningPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_CruisePanel_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CruisePanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CruisePanel_t qt_meta_stringdata_CruisePanel = {
    {
QT_MOC_LITERAL(0, 0, 11) // "CruisePanel"

    },
    "CruisePanel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CruisePanel[] = {

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

void CruisePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CruisePanel::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CruisePanel.data,
    qt_meta_data_CruisePanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CruisePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CruisePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CruisePanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CruisePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_CValueControl_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CValueControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CValueControl_t qt_meta_stringdata_CValueControl = {
    {
QT_MOC_LITERAL(0, 0, 13) // "CValueControl"

    },
    "CValueControl"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CValueControl[] = {

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

void CValueControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CValueControl::staticMetaObject = { {
    &AbstractControl::staticMetaObject,
    qt_meta_stringdata_CValueControl.data,
    qt_meta_data_CValueControl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CValueControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CValueControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CValueControl.stringdata0))
        return static_cast<void*>(this);
    return AbstractControl::qt_metacast(_clname);
}

int CValueControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractControl::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_CPrebuiltToggle_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CPrebuiltToggle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CPrebuiltToggle_t qt_meta_stringdata_CPrebuiltToggle = {
    {
QT_MOC_LITERAL(0, 0, 15) // "CPrebuiltToggle"

    },
    "CPrebuiltToggle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPrebuiltToggle[] = {

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

void CPrebuiltToggle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject CPrebuiltToggle::staticMetaObject = { {
    &ToggleControl::staticMetaObject,
    qt_meta_stringdata_CPrebuiltToggle.data,
    qt_meta_data_CPrebuiltToggle,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CPrebuiltToggle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPrebuiltToggle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CPrebuiltToggle.stringdata0))
        return static_cast<void*>(this);
    return ToggleControl::qt_metacast(_clname);
}

int CPrebuiltToggle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ToggleControl::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_TimeZoneSelectCombo_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TimeZoneSelectCombo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TimeZoneSelectCombo_t qt_meta_stringdata_TimeZoneSelectCombo = {
    {
QT_MOC_LITERAL(0, 0, 19) // "TimeZoneSelectCombo"

    },
    "TimeZoneSelectCombo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TimeZoneSelectCombo[] = {

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

void TimeZoneSelectCombo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TimeZoneSelectCombo::staticMetaObject = { {
    &AbstractControl::staticMetaObject,
    qt_meta_stringdata_TimeZoneSelectCombo.data,
    qt_meta_data_TimeZoneSelectCombo,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TimeZoneSelectCombo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TimeZoneSelectCombo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TimeZoneSelectCombo.stringdata0))
        return static_cast<void*>(this);
    return AbstractControl::qt_metacast(_clname);
}

int TimeZoneSelectCombo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractControl::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_UseBaseTorqueToggle_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UseBaseTorqueToggle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UseBaseTorqueToggle_t qt_meta_stringdata_UseBaseTorqueToggle = {
    {
QT_MOC_LITERAL(0, 0, 19) // "UseBaseTorqueToggle"

    },
    "UseBaseTorqueToggle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UseBaseTorqueToggle[] = {

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

void UseBaseTorqueToggle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject UseBaseTorqueToggle::staticMetaObject = { {
    &ToggleControl::staticMetaObject,
    qt_meta_stringdata_UseBaseTorqueToggle.data,
    qt_meta_data_UseBaseTorqueToggle,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UseBaseTorqueToggle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UseBaseTorqueToggle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UseBaseTorqueToggle.stringdata0))
        return static_cast<void*>(this);
    return ToggleControl::qt_metacast(_clname);
}

int UseBaseTorqueToggle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ToggleControl::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_GitHash_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GitHash_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GitHash_t qt_meta_stringdata_GitHash = {
    {
QT_MOC_LITERAL(0, 0, 7) // "GitHash"

    },
    "GitHash"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GitHash[] = {

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

void GitHash::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject GitHash::staticMetaObject = { {
    &AbstractControl::staticMetaObject,
    qt_meta_stringdata_GitHash.data,
    qt_meta_data_GitHash,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GitHash::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GitHash::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GitHash.stringdata0))
        return static_cast<void*>(this);
    return AbstractControl::qt_metacast(_clname);
}

int GitHash::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractControl::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
