/********************************************************************************
** Form generated from reading UI file 'LoginScreen.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINSCREEN_H
#define UI_LOGINSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QLineEdit *LoginInput;
    QLineEdit *PwdInput;
    QPushButton *LoginButton;
    QLabel *background;
    QPushButton *themeSwitchButton;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName("Client");
        Client->setEnabled(true);
        Client->resize(1440, 1024);
        Client->setMinimumSize(QSize(1440, 1024));
        Client->setMaximumSize(QSize(1440, 1024));
        LoginInput = new QLineEdit(Client);
        LoginInput->setObjectName("LoginInput");
        LoginInput->setGeometry(QRect(720, 500, 600, 70));
        LoginInput->setMinimumSize(QSize(104, 70));
        LoginInput->setBaseSize(QSize(600, 70));
        PwdInput = new QLineEdit(Client);
        PwdInput->setObjectName("PwdInput");
        PwdInput->setGeometry(QRect(720, 590, 600, 70));
        PwdInput->setMinimumSize(QSize(104, 70));
        PwdInput->setBaseSize(QSize(600, 70));
        LoginButton = new QPushButton(Client);
        LoginButton->setObjectName("LoginButton");
        LoginButton->setGeometry(QRect(880, 730, 231, 81));
        background = new QLabel(Client);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 1440, 1024));
        background->setPixmap(QPixmap(QString::fromUtf8(":/images/light/auth_screen.png")));
        background->setScaledContents(true);
        themeSwitchButton = new QPushButton(Client);
        themeSwitchButton->setObjectName("themeSwitchButton");
        themeSwitchButton->setGeometry(QRect(1320, 0, 121, 151));
        themeSwitchButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        background->raise();
        LoginInput->raise();
        PwdInput->raise();
        LoginButton->raise();
        themeSwitchButton->raise();

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "Client", nullptr));
        LoginInput->setText(QString());
        LoginInput->setPlaceholderText(QCoreApplication::translate("Client", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        PwdInput->setPlaceholderText(QCoreApplication::translate("Client", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        LoginButton->setText(QCoreApplication::translate("Client", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        background->setText(QString());
        themeSwitchButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSCREEN_H
