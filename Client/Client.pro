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
    src/Profile/ProfileScreen.h \
    ui_ProfileScreen.h \
    ui_LoginScreen.h \
    ui_AccountExitDialog.h

FORMS += \
    src/AccountExitDialog/AccountExitDialog.ui \
    src/Login/LoginScreen.ui \
    src/Profile/ProfileScreen.ui

RESOURCES += Resources.qrc

RC_FILE=images/mushenger.rc

TARGET = mushenger

INCLUDEPATH += \
    src/Client \
    build/6_7_2_mingw64_shared-Debug \

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
