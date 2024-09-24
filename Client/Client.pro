QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# Указываем исходные файлы, соответствующие новой структуре
SOURCES += \
    main.cpp \
    src/Login/LoginScreen.cpp

# Указываем заголовочные файлы
HEADERS += \
    src/Login/LoginScreen.h

# Указываем файлы форм
FORMS += \
    src/Login/LoginScreen.ui

# Файл ресурсов
RESOURCES += \
    Resources.qrc

# Указание файла ресурсов для иконки
RC_FILE = images/img_icon.rc

# Указываем целевое имя приложения
TARGET = mushenger

INCLUDEPATH += src/Client


# Правила для установки (по умолчанию оставим без изменений)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
