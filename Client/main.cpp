#include "src/Login/LoginScreen.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "src/Profile/ProfileScreen.h"

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


    // Client w;
    // w.show();
    ProfileScreen profileScreen; // Создаем объект окна профиля
    profileScreen.show(); // Показываем окно профиля
    return a.exec();
}
