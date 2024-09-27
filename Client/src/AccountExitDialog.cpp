#include "AccountExitDialog.h"
#include "ui_AccountExitDialog.h"

AccountExitDialog::AccountExitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountExitDialog)
{
    ui->setupUi(this);
}

AccountExitDialog::~AccountExitDialog()
{
    delete ui;
}
