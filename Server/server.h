#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer(int port);

private:
    QMap<qintptr, QTcpSocket*> clients;           // Сокеты клиентов
    QJsonObject userData;                         // Данные пользователей
    QJsonObject chatData;                         // Данные чатов
    QMap<QString, QString> sessionData;           // Хранение сессий (sessionID -> login)

    void loadUserData();
    void saveUserData();
    void loadChatData();
    void saveChatData();
    void handleCreateChat(QTcpSocket *socket, const QJsonObject &request);
    void handleChatRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleLoginOrRegister(QTcpSocket *socket, const QJsonObject &request);
    void handleAvatarUpload(QTcpSocket *socket, const QJsonObject &request, const QByteArray &data);
    void handleMessageRequest(QTcpSocket *socket, const QJsonObject &request);
    void handleCheckUser(QTcpSocket *socket, const QJsonObject &request);
    void handleGetUserData(QTcpSocket *socket, const QJsonObject &request);  // Новый метод
    void handleChatListRequest(QTcpSocket *socket, const QJsonObject &request);  // Новый метод
    void sendErrorResponse(QTcpSocket *socket, const QString &message);

private slots:
    void incomingConnection(qintptr handle) override;
    void receiveData();
    void clientDisconnected();

signals:
    void playerConnected();
    void getMessage();
};

#endif // SERVER_H
