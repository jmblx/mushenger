#include "ChatScreen.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QMessageBox>

ChatScreen::ChatScreen(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void ChatScreen::setupUI()
{
    chatsList = new QListWidget(this);
    messagesList = new QListWidget(this);
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatsList);
    layout->addWidget(messagesList);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    setLayout(layout);

    connect(chatsList, &QListWidget::itemClicked, this, &ChatScreen::onChatSelected);
    connect(sendButton, &QPushButton::clicked, this, &ChatScreen::onSendMessageClicked);
}

void ChatScreen::loadChats(const QJsonArray &chatsData)
{
    chatsList->clear();
    for (const QJsonValue &chat : chatsData) {
        QJsonObject chatObject = chat.toObject();
        QListWidgetItem *item = new QListWidgetItem();
        // Настраиваем внешний вид элемента списка чатов
        item->setText(chatObject["chatName"].toString()); // Название чата
        item->setData(Qt::UserRole, chatObject["chatID"].toString()); // ID чата для идентификации
        // Если это групповой чат, добавляем иконку
        if (chatObject["isGroup"].toBool()) {
            item->setIcon(QIcon(":/icons/group.png")); // Замените на путь к вашей иконке
        }
        chatsList->addItem(item);
    }
}

void ChatScreen::loadMessages(const QJsonArray &messagesData)
{
    messagesList->clear();
    for (const QJsonValue &message : messagesData) {
        QJsonObject messageObject = message.toObject();
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(messageObject["senderName"].toString() + ": " + messageObject["text"].toString());
        item->setIcon(QIcon(":/avatars/" + messageObject["avatar"].toString())); // Путь к аватарке
        messagesList->addItem(item);
    }
}

void ChatScreen::onChatSelected(QListWidgetItem *item)
{
    currentChatID = item->data(Qt::UserRole).toString();
    // Отправляем запрос на сервер для загрузки сообщений выбранного чата
    // Это может быть сигнал/слот или вызов функции для взаимодействия с сервером
}

void ChatScreen::onSendMessageClicked()
{
    QString messageText = messageInput->text();
    if (messageText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Message cannot be empty");
        return;
    }
    // Отправка сообщения на сервер
    // Также добавьте сообщение в список messagesList для отображения пользователю
    messageInput->clear();
}
