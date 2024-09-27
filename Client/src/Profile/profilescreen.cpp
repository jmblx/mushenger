#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"
#include "src/AccountExitDialog/AccountExitDialog.h"

ProfileScreen::ProfileScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen)
{
    ui->setupUi(this);

    connect(ui->overlayButton, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonClicked);

    if (ui->logoutWidget && ui->overlayButton) {
        ui->overlayButton->setGeometry(ui->logoutWidget->geometry());
    }
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
}

void ProfileScreen::onOverlayButtonClicked()
{
    AccountExitDialog dialog(this);
    dialog.setModal(true);

    if (dialog.exec() == QDialog::Accepted) {
        qDebug() << "Кнопка 'Да' нажата";
    } else {
        qDebug() << "Кнопка 'Нет' нажата";
    }
}
