#ifndef ACCOUNTEXITDIALOG_H
#define ACCOUNTEXITDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class AccountExitDialog;
}

class AccountExitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountExitDialog(const QString &sessionID, QWidget *parent = nullptr);
    ~AccountExitDialog();

signals:
    void logoutSuccessful();

private slots:
    void onThemeChanged(const QString& newTheme);
    void onYesButtonClicked();
    void clearUserData();
    void showLoginScreen();

private:
    Ui::AccountExitDialog *ui;

    QString currentSessionID;
    QJsonObject userData;
};

#endif // ACCOUNTEXITDIALOG_H
