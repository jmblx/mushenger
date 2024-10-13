// server.h

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QString>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QImage>

enum class ClientState {
    Idle,
    ExpectingAvatar
};

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer(int port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void receiveData();
    void clientDisconnected(); // Новый слот для обработки отключений

private:
    // Набор chatID чатов, ожидающих загрузки аватаров
    QSet<QString> pendingAvatarChats;
    void notifyNewChat(const QString &chatID, const QString &chatName, const QSet<QString> &participants, const QString &excludeUser = QString());

    bool isUserAdmin(const QString &chatID, const QString &username);  // Проверка прав администратора

    void handleDeleteChat(QTcpSocket *socket, const QJsonObject &request);  // Обработка запроса на удаление чата
    void handleRenameChat(QTcpSocket *socket, const QJsonObject &request);  // Обработка запроса на переименование чата

    void loadChatData();
    void loadUserData();
    QImage image;

    // Обработка различных типов запросов
    void handleCreateChat(QTcpSocket *socket, const QJsonObject &request);
    void handleUploadAvatar(QTcpSocket *socket, const QJsonObject &request);
    void handleCheckUser(QTcpSocket *socket, const QJsonObject &request);
    void handleMessageRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleSyncMessages(QTcpSocket *socket, const QJsonObject &request);
    void handleLoginOrRegister(QTcpSocket *socket, const QJsonObject &request);
    void handleGetUserData(QTcpSocket *socket, const QJsonObject &request);
    void handleChatListRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleGetAvatar(QTcpSocket *socket, const QJsonObject &request);
    void handleLoadMessages(QTcpSocket *socket, const QJsonObject &request);

    void handleGetUserAvatars(QTcpSocket *socket, const QJsonObject &request);

    void sendErrorResponse(QTcpSocket *socket, const QString &message);
    void saveChatData();
    void saveUserData();

    // Хранение состояний клиентов
    QHash<qintptr, ClientState> clientStates;
    QHash<qintptr, QString> clientPendingChatName;
    QHash<qintptr, QString> clientPendingChatID; // Для хранения chatID при загрузке аватара

    // Хранение данных чатов и пользователей
    QHash<QString, QJsonObject> chatData;
    QHash<QString, QJsonObject> userData;
    QHash<QString, QString> sessionData; // sessionID -> username
    QMap<QString, QTcpSocket*> onlineClients; // username -> socket

    // Буферы для хранения неполных данных от клиентов
    QHash<qintptr, QByteArray> clientBuffers;
};

#endif // SERVER_H
