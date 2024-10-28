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
    void clientDisconnected();

private:
    QSet<QString> pendingAvatarChats;
    void notifyNewChat(const QString &chatID, const QString &chatName, const QSet<QString> &participants, const QString &excludeUser = QString());

    bool isUserAdmin(const QString &chatID, const QString &username);

    void handleDeleteChat(QTcpSocket *socket, const QJsonObject &request);
    void handleRenameChat(QTcpSocket *socket, const QJsonObject &request);

    void loadChatData();
    void loadUserData();
    QImage image;

    void handleCreateChat(QTcpSocket *socket, const QJsonObject &request);
    void handleUploadAvatar(QTcpSocket *socket, const QJsonObject &request);
    void handleUploadProfileAvatar(QTcpSocket *socket, const QJsonObject &request);
    void handleCheckUser(QTcpSocket *socket, const QJsonObject &request);
    void handleMessageRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleSyncMessages(QTcpSocket *socket, const QJsonObject &request);
    void handleLoginOrRegister(QTcpSocket *socket, const QJsonObject &request);
    void handleGetUserData(QTcpSocket *socket, const QJsonObject &request);
    void handleChatListRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleGetAvatar(QTcpSocket *socket, const QJsonObject &request);
    void handleLoadMessages(QTcpSocket *socket, const QJsonObject &request);
    void handleLogout(QTcpSocket *socket, const QJsonObject &request);

    void handleGetUserAvatars(QTcpSocket *socket, const QJsonObject &request);

    void sendErrorResponse(QTcpSocket *socket, const QString &message);
    void saveChatData();
    void saveUserData();

    QHash<qintptr, ClientState> clientStates;
    QHash<qintptr, QString> clientPendingChatName;
    QHash<qintptr, QString> clientPendingChatID;

    QHash<QString, QJsonObject> chatData;
    QHash<QString, QJsonObject> userData;
    QHash<QString, QString> sessionData;
    QMap<QString, QTcpSocket*> onlineClients;

    QHash<qintptr, QByteArray> clientBuffers;
};

#endif // SERVER_H
