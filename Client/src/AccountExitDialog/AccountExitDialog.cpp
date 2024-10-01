#include "AccountExitDialog.h"
#include "ui_AccountExitDialog.h"
#include "src/ThemeManager/ThemeManager.h"
#include <QDebug>

AccountExitDialog::AccountExitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountExitDialog)
{
    ui->setupUi(this);

    // Connect to ThemeManager
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &AccountExitDialog::onThemeChanged);
    // Apply the current theme
    onThemeChanged(ThemeManager::instance().currentTheme());

    connect(ui->yesButton, &QPushButton::clicked, this, [this](){
        qDebug() << "Account logout initiated.";
        accept(); // Close the dialog with accept result
    });

    connect(ui->noButton, &QPushButton::clicked, this, &QDialog::reject);
}

AccountExitDialog::~AccountExitDialog()
{
    delete ui;
}

void AccountExitDialog::onThemeChanged(const QString& newTheme)
{
    // Update background image
    QString backgroundPath = QString(":/images/%1/account_exit_dialog_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);
    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Failed to load background image:" << backgroundPath;
    }
}
