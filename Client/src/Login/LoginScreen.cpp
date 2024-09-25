//made by kevinwu06
#include "src/Login/LoginScreen.h"
#include "src/Login/ui_LoginScreen.h"
#include <QDebug>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

}

void Client::receiveData()
{
    // Заглушка для функции получения данных
}

void Client::on_connectButton_clicked()
{
    // Заглушка для обработки нажатия кнопки подключения
}

void Client::on_sendButton_clicked()
{
    // Заглушка для обработки нажатия кнопки отправки
}

void Client::next()
{
    // Заглушка для функции next
}
