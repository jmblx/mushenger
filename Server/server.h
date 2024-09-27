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
    void Connect(int port);

private:
    QMap<qintptr, QTcpSocket*> clients; // Контейнер для хранения сокетов клиентов
    QJsonObject userData;               // Данные пользователей (логин и пароль)
    QMap<QString, QString> sessionData; // Хранение сессий (sessionID -> login)

    void loadUserData();
    void saveUserData();

private slots:
    void incomingConnection(qintptr handle) override; // Обработка нового подключения
    void receiveData();                               // Обработка данных от клиента
    void clientDisconnected();

signals:
    void playerConnected();
    void getMessage();
};

#endif // SERVER_H
