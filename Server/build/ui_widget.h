/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QLineEdit *port;
    QPushButton *startButton;
    QLineEdit *nameEdit;
    QLineEdit *send;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(380, 400);
        Widget->setMinimumSize(QSize(380, 400));
        Widget->setMaximumSize(QSize(380, 400));
        Widget->setStyleSheet(QString::fromUtf8("QWidget{\n"
"border: none;\n"
"}\n"
"QLineEdit {\n"
"      border: 0.5px solid rgb(147, 150, 154);\n"
"      border-radius: 6px;\n"
"      background-color: rgba(40, 44, 52,150);\n"
"      min-width: 80px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:11pt;\n"
"	  font-weight: bold;\n"
"	  color:rgb(147, 150, 154);\n"
"  }"));
        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(10, 61, 361, 261));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font.setPointSize(11);
        font.setBold(true);
        textEdit->setFont(font);
        textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"      border: 0.5px solid rgb(147, 150, 154);\n"
"      border-radius: 6px;\n"
"      background-color: rgba(40, 44, 52, 150);\n"
"      min-width: 80px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:11pt;\n"
"	  font-weight: bold;\n"
"	  color:rgb(215, 215, 215);\n"
"  }"));
        textEdit->setLineWidth(2);
        textEdit->setReadOnly(true);
        textEdit->setOverwriteMode(false);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(290, 330, 82, 61));
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("  QPushButton {\n"
"      border: 0.5px solid white;\n"
"      border-radius: 6px;\n"
"      background-color: rgb(90,194,198);\n"
"      min-width: 80px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:11pt;\n"
"	  font-weight: bold;\n"
"	  color:white;\n"
"  }\n"
" QPushButton:hover {\n"
"	border: 0.5px solid white;\n"
"      border-radius: 6px;\n"
"      background-color: #1fab89;\n"
"      min-width: 80px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:10pt;\n"
"	  font-weight: bold;\n"
"	  color:white;\n"
" }"));
        port = new QLineEdit(Widget);
        port->setObjectName("port");
        port->setGeometry(QRect(10, 10, 141, 31));
        port->setFont(font);
        startButton = new QPushButton(Widget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(160, 10, 61, 31));
        startButton->setFont(font);
        startButton->setStyleSheet(QString::fromUtf8("  QPushButton {\n"
"      border: 0.5px solid white;\n"
"      border-radius: 6px;\n"
"      background-color: rgb(90,194,198);\n"
"      min-width: 40px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:11pt;\n"
"	  font-weight: bold;\n"
"	  color:white;\n"
"  }\n"
" QPushButton:hover {\n"
"	border: 0.5px solid white;\n"
"      border-radius: 6px;\n"
"      background-color: #1fab89;\n"
"      min-width: 40px;\n"
"	  font-family: \"Microsoft YaHei\";\n"
"	  font-size:10pt;\n"
"	  font-weight: bold;\n"
"	  color:white;\n"
" }"));
        nameEdit = new QLineEdit(Widget);
        nameEdit->setObjectName("nameEdit");
        nameEdit->setGeometry(QRect(230, 10, 141, 31));
        nameEdit->setFont(font);
        send = new QLineEdit(Widget);
        send->setObjectName("send");
        send->setGeometry(QRect(10, 330, 271, 61));
        send->setFont(font);
        send->setStyleSheet(QString::fromUtf8("QLineEdit\n"
"{\n"
"color:rgb(215, 215, 215);\n"
"}"));
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 381, 401));
        label->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"	border-image: url(:/Images/duckl.png);\n"
"}"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(12, 43, 231, 16));
        label_2->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"color:rgb(147, 150, 154);\n"
"}"));
        label->raise();
        textEdit->raise();
        pushButton->raise();
        port->raise();
        startButton->raise();
        nameEdit->raise();
        send->raise();
        label_2->raise();

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "Send", nullptr));
        port->setPlaceholderText(QCoreApplication::translate("Widget", "Port\347\253\257\345\217\243", nullptr));
        startButton->setText(QCoreApplication::translate("Widget", "Start", nullptr));
        nameEdit->setText(QString());
        nameEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\346\230\276\347\244\272\345\220\215\347\247\260", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("Widget", "\345\274\200\345\217\221\350\200\205:Kevinwu  Github:kevinwu06", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
