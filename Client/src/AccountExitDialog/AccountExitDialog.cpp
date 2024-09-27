#include "AccountExitDialog.h"
#include "ui_AccountExitDialog.h"

AccountExitDialog::AccountExitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountExitDialog)
{
    ui->setupUi(this);

    connect(ui->yesButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->noButton, &QPushButton::clicked, this, &QDialog::reject);
}

AccountExitDialog::~AccountExitDialog()
{
    delete ui;
}
