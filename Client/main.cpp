#include <QApplication>
#include <QFile>
#include <QDebug>
#include "src/Profile/ProfileScreen.h"
#include "src/Login/LoginScreen.h"
#include "src/ThemeManager/ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ThemeManager::instance().setTheme(ThemeManager::instance().currentTheme());

    LoginScreen w;
    w.show();

    return a.exec();
}
