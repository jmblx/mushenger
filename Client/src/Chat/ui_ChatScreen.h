/********************************************************************************
** Form generated from reading UI file 'ChatScreen.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATSCREEN_H
#define UI_CHATSCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatScreen
{
public:
    QListWidget *chatsList;
    QLabel *label;
    QLabel *userAvatar;
    QListWidget *messagesList;
    QWidget *widget;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *newChat;
    QLabel *chatsLabel;
    QLabel *chatCount;
    QLabel *userName;

    void setupUi(QWidget *ChatScreen)
    {
        if (ChatScreen->objectName().isEmpty())
            ChatScreen->setObjectName("ChatScreen");
        ChatScreen->resize(1440, 1024);
        chatsList = new QListWidget(ChatScreen);
        chatsList->setObjectName("chatsList");
        chatsList->setGeometry(QRect(30, 180, 401, 851));
        label = new QLabel(ChatScreen);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 1440, 1024));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/chat_screen.png")));
        userAvatar = new QLabel(ChatScreen);
        userAvatar->setObjectName("userAvatar");
        userAvatar->setGeometry(QRect(30, 20, 90, 90));
        userAvatar->setPixmap(QPixmap(QString::fromUtf8(":/images/profile-circled.svg")));
        userAvatar->setScaledContents(true);
        messagesList = new QListWidget(ChatScreen);
        messagesList->setObjectName("messagesList");
        messagesList->setGeometry(QRect(460, 130, 880, 771));
        widget = new QWidget(ChatScreen);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(510, 920, 861, 71));
        messageInput = new QLineEdit(widget);
        messageInput->setObjectName("messageInput");
        messageInput->setGeometry(QRect(0, 0, 861, 70));
        messageInput->setMinimumSize(QSize(104, 70));
        messageInput->setBaseSize(QSize(600, 70));
        messageInput->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
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
        sendButton = new QPushButton(widget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(790, 20, 33, 27));
        sendButton->setAutoFillBackground(false);
        sendButton->setStyleSheet(QString::fromUtf8("QPushButton#sendButton {\n"
"    background: transparent;\n"
"    border: none;\n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/send_message_icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        sendButton->setIcon(icon);
        newChat = new QPushButton(ChatScreen);
        newChat->setObjectName("newChat");
        newChat->setGeometry(QRect(250, 130, 151, 41));
        newChat->setStyleSheet(QString::fromUtf8("QPushButton#newChat {\n"
"    font-size: 26px;              /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\202\320\265\320\272\321\201\321\202\320\260 */\n"
"    color: #000000;               /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 \342\200\224 \321\207\320\265\321\200\320\275\321\213\320\271 */\n"
"    background-color: #FFFFFF;    /* \320\244\320\276\320\275\320\276\320\262\321\213\320\271 \321\206\320\262\320\265\321\202 \342\200\224 \320\261\320\265\320\273\321\213\320\271 */\n"
"    border: 2px solid #000000;    /* \320\247\320\265\321\200\320\275\320\260\321\217 \320\263\321\200\320\260\320\275\320\270\321\206\320\260 (\320\276\320\277\321\206\320\270\320\276\320\275\320\260\320\273\321\214\320\275\320\276) */\n"
"    border-radius: 15px;          /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \320\263\321\200\320\260\320\275\320\270\321\206 */\n"
"    width: 210px;                 /* \320\250\320\270\321\200"
                        "\320\270\320\275\320\260 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    height: 50px;                 /* \320\222\321\213\321\201\320\276\321\202\320\260 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    padding: 0;                   /* \320\243\320\261\320\270\321\200\320\260\320\265\320\274 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 */\n"
"}\n"
""));
        chatsLabel = new QLabel(ChatScreen);
        chatsLabel->setObjectName("chatsLabel");
        chatsLabel->setGeometry(QRect(30, 140, 71, 20));
        chatsLabel->setStyleSheet(QString::fromUtf8("QLabel#chatsLabel {\n"
"	font-size: 20px;              /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\202\320\265\320\272\321\201\321\202\320\260 */\n"
"    color: #000000;               /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 \342\200\224 \321\207\320\265\321\200\320\275\321\213\320\271 */\n"
"	font-weight: bold;\n"
"}"));
        chatCount = new QLabel(ChatScreen);
        chatCount->setObjectName("chatCount");
        chatCount->setGeometry(QRect(100, 137, 30, 30));
        chatCount->setStyleSheet(QString::fromUtf8("QLabel#chatCount {\n"
"    font-size: 14px;              /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
"    font-weight: bold;            /* \320\226\320\270\321\200\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    color: #000000;               /* \320\246\320\262\320\265\321\202 \321\202\320\265\320\272\321\201\321\202\320\260 \342\200\224 \321\207\320\265\321\200\320\275\321\213\320\271 */\n"
"    background-color: #FFFFFF;    /* \320\246\320\262\320\265\321\202 \321\204\320\276\320\275\320\260 \342\200\224 \320\261\320\265\320\273\321\213\320\271 */\n"
"    border-radius: 15px;          /* \320\227\320\260\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \320\263\321\200\320\260\320\275\320\270\321\206 \320\264\320\273\321\217 \320\272\321\200\321\203\320\263\320\260 */\n"
"    min-width: 30px;              /* \320\234\320\270\320\275\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 "
                        "\321\210\320\270\321\200\320\270\320\275\320\260 \320\264\320\273\321\217 \320\272\321\200\321\203\320\263\320\260 */\n"
"    min-height: 30px;             /* \320\234\320\270\320\275\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \320\262\321\213\321\201\320\276\321\202\320\260 \320\264\320\273\321\217 \320\272\321\200\321\203\320\263\320\260 */\n"
"    max-width: 30px;              /* \320\234\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \321\210\320\270\321\200\320\270\320\275\320\260 \320\264\320\273\321\217 \320\272\321\200\321\203\320\263\320\260 */\n"
"    max-height: 30px;             /* \320\234\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \320\262\321\213\321\201\320\276\321\202\320\260 \320\264\320\273\321\217 \320\272\321\200\321\203\320\263\320\260 */\n"
"    padding: 0;                   /* \320\243\320\261\320\270\321\200\320\260\320\265\320\274 \320\276\321\202\321\201\321\202\321\203\320\277\321"
                        "\213 \320\264\320\273\321\217 \321\206\320\265\320\275\321\202\321\200\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217 */\n"
"    text-align: center;           /* \320\246\320\265\320\275\321\202\321\200\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\202\320\265\320\272\321\201\321\202\320\260 \320\277\320\276 \320\263\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\320\270 */\n"
"}\n"
""));
        chatCount->setScaledContents(false);
        chatCount->setAlignment(Qt::AlignmentFlag::AlignCenter);
        userName = new QLabel(ChatScreen);
        userName->setObjectName("userName");
        userName->setGeometry(QRect(130, 60, 171, 31));
        QFont font;
        userName->setFont(font);
        userName->setStyleSheet(QString::fromUtf8("QLabel#userName {\n"
"	    font-size: 22px;              /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260 */            /* \320\226\320\270\321\200\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    color: #000000; \n"
"}"));
        label->raise();
        chatsList->raise();
        userAvatar->raise();
        messagesList->raise();
        widget->raise();
        newChat->raise();
        chatsLabel->raise();
        chatCount->raise();
        userName->raise();

        retranslateUi(ChatScreen);

        QMetaObject::connectSlotsByName(ChatScreen);
    } // setupUi

    void retranslateUi(QWidget *ChatScreen)
    {
        ChatScreen->setWindowTitle(QCoreApplication::translate("ChatScreen", "Form", nullptr));
        label->setText(QString());
        userAvatar->setText(QString());
        messageInput->setText(QString());
        messageInput->setPlaceholderText(QCoreApplication::translate("ChatScreen", "  \320\235\320\260\320\277\320\270\321\210\320\270\321\202\320\265 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\265...", nullptr));
        sendButton->setText(QString());
        newChat->setText(QCoreApplication::translate("ChatScreen", "\320\235\320\276\320\262\321\213\320\271 \321\207\320\260\321\202", nullptr));
        chatsLabel->setText(QCoreApplication::translate("ChatScreen", "\320\247\320\260\321\202\320\276\320\262:", nullptr));
        chatCount->setText(QCoreApplication::translate("ChatScreen", "0", nullptr));
        userName->setText(QCoreApplication::translate("ChatScreen", "username", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatScreen: public Ui_ChatScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATSCREEN_H
