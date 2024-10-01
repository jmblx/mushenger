#ifndef ACCOUNTEXITDIALOG_H
#define ACCOUNTEXITDIALOG_H

#include <QDialog>

namespace Ui {
class AccountExitDialog;
}

class AccountExitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountExitDialog(QWidget *parent = nullptr);
    ~AccountExitDialog();

private slots:
    void onThemeChanged(const QString& newTheme);

private:
    Ui::AccountExitDialog *ui;
};

#endif // ACCOUNTEXITDIALOG_H
