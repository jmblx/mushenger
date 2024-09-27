#include <QApplication>
#include <QFile>
#include <QDebug>
#include "src/Profile/ProfileScreen.h"
#include "src/Login/LoginScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ProfileScreen profileScreen;
    profileScreen.show();

    // LoginScreen w;
    // w.show();

    return a.exec();
}
