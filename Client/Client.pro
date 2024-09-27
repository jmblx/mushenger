QT       += core gui network
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += \
    main.cpp \
    src/AccountExitDialog/AccountExitDialog.cpp \
    src/Login/LoginScreen.cpp \
    src/Profile/ProfileScreen.cpp \

HEADERS += \
    src/AccountExitDialog/AccountExitDialog.h \
    src/Login/LoginScreen.h \
    src/Login/ui_LoginScreen.h \
    src/Profile/ProfileScreen.h \
    build/6_7_2_mingw_shared-Debug/build/6_7_2_mingw_shared-Debug/ui_ProfileScreen.h \
    build/6_7_2_mingw_shared-Debug/build/6_7_2_mingw_shared-Debug/ui_LoginScreen.h

FORMS += \
    src/AccountExitDialog/AccountExitDialog.ui \
    src/Login/LoginScreen.ui \
    src/Profile/ProfileScreen.ui

RESOURCES += Resources.qrc

RC_FILE=images/mushenger.rc

TARGET = mushenger

INCLUDEPATH += src/Client

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
