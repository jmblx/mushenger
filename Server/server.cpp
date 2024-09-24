#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer{parent}
{
}

void Server::Connect(int port)
{
    listen(QHostAddress::Any, port); // Начинаем прослушивать входящие подключения
}

void Server::incomingConnection(qintptr handle)
{
    QTcpSocket *clientSocket = new QTcpSocket(this); // Создаем новый сокет для каждого клиента
    clientSocket->setSocketDescriptor(handle); // Настраиваем сокет на управление новым соединением

    clients.insert(handle, clientSocket); // Добавляем сокет в контейнер клиентов

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::receiveData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    emit playerConnected(); // Сигнал о подключении нового клиента
}

void Server::receiveData()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender()); // Определяем, от какого клиента пришли данные
    if (!senderSocket) return;

    QByteArray message = senderSocket->readAll();
    str = message.data(); // Сохраняем последнее полученное сообщение

    emit getMessage(); // Испускаем сигнал о получении сообщения

    // Рассылаем сообщение всем клиентам
    for (QTcpSocket *clientSocket : clients) {
        if (clientSocket != senderSocket) { // Исключаем отправку сообщения отправителю
            clientSocket->write(message); // Отправляем сообщение остальным клиентам
        }
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); // Определяем, какой клиент отключился
    if (!clientSocket) return;

    clients.remove(clientSocket->socketDescriptor()); // Удаляем сокет из контейнера
    clientSocket->deleteLater(); // Удаляем сокет после завершения всех его операций
}

void Server::sendData(QString text)
{
    // Отправка данных всем клиентам
    for (QTcpSocket *clientSocket : clients) {
        clientSocket->write(text.toUtf8());
    }
}

QString Server::getContent()
{
    return str; // Возвращает последнее полученное сообщение
}
