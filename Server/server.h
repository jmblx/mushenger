#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void Connect(int port);
    QString getContent();
    void sendData(QString text);

private:
    QMap<qintptr, QTcpSocket*> clients; // Контейнер для хранения сокетов клиентов
    QString str;

private slots:
    void incomingConnection(qintptr handle) override; // Обработка нового подключения
    void receiveData(); // Обработка данных от клиента
    void clientDisconnected(); // Обработка отключения клиента

signals:
    void playerConnected();
    void getMessage();
};

#endif // SERVER_H
