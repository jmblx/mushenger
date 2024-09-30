#include "AccountExitDialog.h"
#include "ui_AccountExitDialog.h"
#include <QDebug>

AccountExitDialog::AccountExitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountExitDialog)
{
    ui->setupUi(this);

    connect(ui->yesButton, &QPushButton::clicked, this, [this](){
        qDebug() << "Выход из аккаунта должен был произойти.";
    });

    connect(ui->noButton, &QPushButton::clicked, this, &QDialog::reject);
}

AccountExitDialog::~AccountExitDialog()
{
    delete ui;
}
