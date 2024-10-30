/****************************************************************************
** Meta object code from reading C++ file 'ProfileScreen.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Profile/ProfileScreen.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProfileScreen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSProfileScreenENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSProfileScreenENDCLASS = QtMocHelpers::stringData(
    "ProfileScreen",
    "onOverlayButtonClicked",
    "",
    "onBackButtonClicked",
    "onThemeSwitchButtonClicked",
    "onThemeChanged",
    "newTheme",
    "onReadyRead",
    "onOverlayButtonHoverEntered",
    "onOverlayButtonHoverLeft",
    "onProfileButtonClicked",
    "onLogout",
    "displayUserAvatar",
    "avatar"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSProfileScreenENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    1 /* Private */,
       3,    0,   75,    2, 0x08,    2 /* Private */,
       4,    0,   76,    2, 0x08,    3 /* Private */,
       5,    1,   77,    2, 0x08,    4 /* Private */,
       7,    0,   80,    2, 0x08,    6 /* Private */,
       8,    0,   81,    2, 0x08,    7 /* Private */,
       9,    0,   82,    2, 0x08,    8 /* Private */,
      10,    0,   83,    2, 0x08,    9 /* Private */,
      11,    0,   84,    2, 0x08,   10 /* Private */,
      12,    1,   85,    2, 0x08,   11 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject ProfileScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSProfileScreenENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSProfileScreenENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSProfileScreenENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ProfileScreen, std::true_type>,
        // method 'onOverlayButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBackButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onThemeSwitchButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onThemeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onOverlayButtonHoverEntered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onOverlayButtonHoverLeft'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProfileButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLogout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayUserAvatar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>
    >,
    nullptr
} };

void ProfileScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProfileScreen *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onOverlayButtonClicked(); break;
        case 1: _t->onBackButtonClicked(); break;
        case 2: _t->onThemeSwitchButtonClicked(); break;
        case 3: _t->onThemeChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onReadyRead(); break;
        case 5: _t->onOverlayButtonHoverEntered(); break;
        case 6: _t->onOverlayButtonHoverLeft(); break;
        case 7: _t->onProfileButtonClicked(); break;
        case 8: _t->onLogout(); break;
        case 9: _t->displayUserAvatar((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ProfileScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSProfileScreenENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
