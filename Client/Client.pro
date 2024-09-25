QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += \
    main.cpp \
    src/Login/LoginScreen.cpp \
    src/Profile/ProfileScreen.cpp \

HEADERS += \
    src/Login/LoginScreen.h \
    src/Login/ui_LoginScreen.h \
    src/Profile/ProfileScreen.h \
    src/Profile/ui_ProfileScreen.h

FORMS += \
    src/Login/LoginScreen.ui \
    src/Profile/ProfileScreen.ui

RESOURCES += Resources.qrc


TARGET = mushenger

INCLUDEPATH += src/Client

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
