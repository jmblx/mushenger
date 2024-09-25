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
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *LoginButton;
    QLabel *label;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName("Client");
        Client->setEnabled(true);
        Client->resize(1440, 1024);
        Client->setMinimumSize(QSize(1440, 1024));
        Client->setMaximumSize(QSize(1440, 1024));
        Client->setAutoFillBackground(true);
        Client->setStyleSheet(QString::fromUtf8("#Client {\n"
                                                "    background-image: url(:/images/auth_screen.png);\n"
                                                "    background-repeat: no-repeat;\n"
                                                "    background-position: center;\n"
                                                "}\n"
                                                "\n"
                                                "QLineEdit {\n"
                                                "      border: 0.5px solid rgb(147, 150, 154);\n"
                                                "      border-radius: 6px;\n"
                                                "\n"
                                                "      min-width: 80px;\n"
                                                "	  font-family: \"Microsoft YaHei\";\n"
                                                "	  font-size:11pt;\n"
                                                "	  font-weight: bold;\n"
                                                "	  color:rgb(147, 150, 154);\n"
                                                "  }"));
        lineEdit = new QLineEdit(Client);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(720, 490, 600, 70));
        lineEdit->setMinimumSize(QSize(104, 70));
        lineEdit->setBaseSize(QSize(600, 70));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
                                                  "    border: 2px solid #0D1321;     /* \320\246\320\262\320\265\321\202 \320\276\320\261\320\262\320\276\320\264\320\272\320\270 */\n"
                                                  "    border-radius: 10px;           /* \320\241\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
                                                  "    padding: 10px;                 /* \320\236\321\202\321\201\321\202\321\203\320\277 \320\262\320\275\321\203\321\202\321\200\320\270 \320\277\320\276\320\273\321\217 */\n"
                                                  "    font-size: 20px;               /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\202\320\265\320\272\321\201\321\202\320\260 \320\264\320\273\321\217 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263\320\276 \321\202\320\265\320\272\321\201\321\202\320\260 \320\270 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                  "    color: #0D1321;                /* \320\246\320\262\320\265\321\202 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263"
                                                  "\320\276 \321\202\320\265\320\272\321\201\321\202\320\260 */\n"
                                                  "    background: transparent;       /* \320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
                                                  "}\n"
                                                  "\n"
                                                  "QLineEdit::placeholder {\n"
                                                  "    color: #0D1321;                /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                  "    background: transparent;       /* \320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\321\213\320\271 \321\204\320\276\320\275 \320\264\320\273\321\217 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                  "}\n"
                                                  ""));
        lineEdit_2 = new QLineEdit(Client);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(720, 590, 600, 70));
        lineEdit_2->setMinimumSize(QSize(104, 70));
        lineEdit_2->setBaseSize(QSize(600, 70));
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
                                                    "    border: 2px solid #0D1321;     /* \320\246\320\262\320\265\321\202 \320\276\320\261\320\262\320\276\320\264\320\272\320\270 */\n"
                                                    "    border-radius: 10px;           /* \320\241\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
                                                    "    padding: 10px;                 /* \320\236\321\202\321\201\321\202\321\203\320\277 \320\262\320\275\321\203\321\202\321\200\320\270 \320\277\320\276\320\273\321\217 */\n"
                                                    "    font-size: 20px;               /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\202\320\265\320\272\321\201\321\202\320\260 \320\264\320\273\321\217 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263\320\276 \321\202\320\265\320\272\321\201\321\202\320\260 \320\270 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                    "    color: #0D1321;                /* \320\246\320\262\320\265\321\202 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263"
                                                    "\320\276 \321\202\320\265\320\272\321\201\321\202\320\260 */\n"
                                                    "    background: transparent;       /* \320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
                                                    "}\n"
                                                    "\n"
                                                    "QLineEdit::placeholder {\n"
                                                    "    color: #0D1321;                /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                    "    background: transparent;       /* \320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\321\213\320\271 \321\204\320\276\320\275 \320\264\320\273\321\217 \320\277\320\273\320\265\320\271\321\201\321\205\320\276\320\273\320\264\320\265\321\200\320\260 */\n"
                                                    "}\n"
                                                    ""));
        LoginButton = new QPushButton(Client);
        LoginButton->setObjectName("LoginButton");
        LoginButton->setGeometry(QRect(880, 730, 231, 81));
        LoginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
                                                     "    background-color: #C21B1B;  /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 */\n"
                                                     "    color: white;        /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 */\n"
                                                     "    font-size: 24px;            /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
                                                     "    border-radius: 30px;        /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
                                                     "    padding: 10px 20px;         /* \320\222\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
                                                     "    background: transparent;       /* \320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
                                                     "    background-color: #C21B1B; \n"
                                                     "}\n"
                                                     "\n"
                                                     "QPushButton:hover {\n"
                                                     "    background-color: #A51A1A;  /* \320\230\320\267\320\274\320\265\320"
                                                     "\275\320\265\320\275\320\270\320\265 \321\206\320\262\320\265\321\202\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
                                                     "}\n"
                                                     ""));
        label = new QLabel(Client);
        label->setObjectName("label");
        label->setGeometry(QRect(0, -10, 1441, 1041));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/auth_screen.png")));
        label->raise();
        lineEdit->raise();
        lineEdit_2->raise();
        LoginButton->raise();

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "Client", nullptr));
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("Client", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("Client", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        LoginButton->setText(QCoreApplication::translate("Client", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSCREEN_H
