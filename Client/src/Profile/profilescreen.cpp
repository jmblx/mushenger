#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"

ProfileScreen::ProfileScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen) // создаем объект интерфейса
{
    ui->setupUi(this); // связываем интерфейс с виджетом
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
}
