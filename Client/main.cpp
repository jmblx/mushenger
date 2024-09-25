#include "src/Login/LoginScreen.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Проверка доступности файла
    QPixmap pixmap(":/images/auth_screen.png");
    if (pixmap.isNull()) {
        qDebug() << "Не удалось загрузить изображение";
    } else {
        qDebug() << "Изображение успешно загружено";
    }


    Client w;
    w.show();
    return a.exec();
}
