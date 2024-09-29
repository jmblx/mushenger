QT       += core gui network
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# Указываем исходные файлы, соответствующие новой структуре
SOURCES += \
    src/Chat/ChatScreen.cpp \
    main.cpp \
    src/Login/LoginScreen.cpp

# Указываем заголовочные файлы
HEADERS += \
    src/Chat/ChatScreen.h \
    src/Login/LoginScreen.h \

# Указываем файлы форм
FORMS += \
    src/Chat/ChatScreen.ui \
    src/Login/LoginScreen.ui

# Файл ресурсов
RESOURCES += Resources.qrc

RC_FILE=images/mushenger.rc
# Указание файла ресурсов для иконки

# Указываем целевое имя приложения
TARGET = mushenger

INCLUDEPATH += src/Client


# Правила для установки (по умолчанию оставим без изменений)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
