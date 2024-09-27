#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"

ProfileScreen::ProfileScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen)
{
    ui->setupUi(this);
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
}
