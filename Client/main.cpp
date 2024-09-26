#include <QApplication>
#include "src/Login/LoginScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginScreen w; // Создаем объект экрана логина
    w.show();      // Показываем окно
    return a.exec();
}
