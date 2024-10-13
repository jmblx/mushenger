QT       += core gui network
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += \
    src/Chat/ChatScreen.cpp \
    main.cpp \
    src/AccountExitDialog/AccountExitDialog.cpp \
    src/Chat/MessageWidget.cpp \
    src/Chat/chatitemdelegate.cpp \
    src/Login/LoginScreen.cpp \
    src/Profile/ProfileScreen.cpp \

HEADERS += \
    src/AccountExitDialog/AccountExitDialog.h \
    src/Chat/MessageWidget.h \
    src/Chat/chatitemdelegate.h \
    src/Login/LoginScreen.h \
    src/Profile/ProfileScreen.h \
    src/Chat/ChatScreen.h \
    ui_ProfileScreen.h \
    ui_LoginScreen.h \
    ui_AccountExitDialog.h\
    ui_ChatScreen.h\

FORMS += \
    src/AccountExitDialog/AccountExitDialog.ui \
    src/Login/LoginScreen.ui \
    src/Profile/ProfileScreen.ui \
    src/Chat/ChatScreen.ui \
    src/Login/LoginScreen.ui\

RESOURCES += Resources.qrc

RC_FILE=images/mushenger.rc

TARGET = mushenger

INCLUDEPATH += \
    src/Client \
    build/6_7_2_mingw64_shared-Debug \

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target