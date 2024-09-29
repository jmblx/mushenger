#ifndef CHATSCREEN_H
#define CHATSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class ChatScreen;
}

class ChatScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChatScreen(const QString &sessionID, const QString &userLogin, QWidget *parent = nullptr);
    ~ChatScreen();

private slots:
    void onChatSelected(QListWidgetItem *item); // Слот для обработки выбора чата
    void loadChats();                          // Загрузка чатов
    void loadMessages(const QString &chatID);  // Загрузка сообщений для выбранного чата
    void onSendMessageClicked();               // Обработка нажатия кнопки отправки сообщения
    void onReadyRead();                        // Обработка полученных данных от сервера
    void onEnterPressed();                     // Слот для обработки нажатия Enter
    void onNewChatClicked();                   // Слот для создания нового чата
    bool checkUserExists(const QString &username); // Проверка существования пользователя
    void sendAvatarToServer(const QByteArray &avatarData, const QString &chatName); // Отправка аватарки

private:
    Ui::ChatScreen *ui;
    QTcpSocket *socket;      // Сокет для соединения с сервером
    QString sessionID;       // Хранение sessionID
    QString currentChatID;
    QString currentUserLogin;    // Хранение текущего chatID

    void connectToServer();                     // Подключение к серверу
    void sendRequest(const QJsonObject &request); // Отправка запроса на сервер
    QByteArray processAvatar(const QString &filePath); // Обработка аватарки
};

#endif // CHATSCREEN_H
