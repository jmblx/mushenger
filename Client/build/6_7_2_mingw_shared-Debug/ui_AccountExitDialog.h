/********************************************************************************
** Form generated from reading UI file 'AccountExitDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCOUNTEXITDIALOG_H
#define UI_ACCOUNTEXITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AccountExitDialog
{
public:
    QLabel *background;
    QPushButton *yesButton;
    QPushButton *noButton;
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *AccountExitDialog)
    {
        if (AccountExitDialog->objectName().isEmpty())
            AccountExitDialog->setObjectName("AccountExitDialog");
        AccountExitDialog->resize(985, 889);
        AccountExitDialog->setMinimumSize(QSize(985, 889));
        AccountExitDialog->setMaximumSize(QSize(985, 889));
        background = new QLabel(AccountExitDialog);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 985, 889));
        background->setPixmap(QPixmap(QString::fromUtf8(":/images/account_exit_dialog_screen.png")));
        background->setScaledContents(true);
        yesButton = new QPushButton(AccountExitDialog);
        yesButton->setObjectName("yesButton");
        yesButton->setGeometry(QRect(303, 666, 181, 117));
        noButton = new QPushButton(AccountExitDialog);
        noButton->setObjectName("noButton");
        noButton->setGeometry(QRect(502, 666, 181, 117));
        verticalLayout = new QVBoxLayout(AccountExitDialog);
        verticalLayout->setObjectName("verticalLayout");
        yesButton->raise();
        background->raise();
        noButton->raise();

        retranslateUi(AccountExitDialog);

        QMetaObject::connectSlotsByName(AccountExitDialog);
    } // setupUi

    void retranslateUi(QDialog *AccountExitDialog)
    {
        AccountExitDialog->setWindowTitle(QCoreApplication::translate("AccountExitDialog", "\320\222\321\213\321\205\320\276\320\264 \320\270\320\267 \320\260\320\272\320\272\320\260\321\203\320\275\321\202\320\260", nullptr));
        yesButton->setText(QCoreApplication::translate("AccountExitDialog", "\320\224\320\260", nullptr));
        noButton->setText(QCoreApplication::translate("AccountExitDialog", "\320\235\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AccountExitDialog: public Ui_AccountExitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCOUNTEXITDIALOG_H
