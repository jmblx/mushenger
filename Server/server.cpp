#include "server.h"
#include <QDebug>
#include <QJsonDocument>
#include <QUuid>
#include <QFile>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    loadUserData();
}

void Server::Connect(int port)
{
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Failed to start server on port" << port << ":" << this->errorString();
    }
}

void Server::incomingConnection(qintptr handle)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(handle);
    clients[handle] = clientSocket;

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::receiveData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    qDebug() << "New client connected with handle" << handle;
}

void Server::receiveData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QJsonDocument requestDoc = QJsonDocument::fromJson(data);
    if (!requestDoc.isObject()) {
        clientSocket->write("Invalid JSON format.");
        return;
    }

    QJsonObject request = requestDoc.object();
    QString login = request.value("login").toString();
    QString password = request.value("password").toString();
    QString sessionID = request.value("sessionID").toString();

    QJsonObject response;

    // Проверка сессии
    if (!sessionID.isEmpty() && sessionData.contains(sessionID)) {
        QString storedLogin = sessionData[sessionID];
        response["status"] = "success";
        response["message"] = "Session valid.";
        response["user"] = storedLogin;
        clientSocket->write(QJsonDocument(response).toJson());
        return;
    }

    // Регистрация и авторизация
    if (userData.contains(login)) {
        if (userData[login] == password) {
            QString newSessionID = QUuid::createUuid().toString(QUuid::WithoutBraces);
            sessionData[newSessionID] = login;
            response["status"] = "success";
            response["message"] = "Login successful.";
            response["sessionID"] = newSessionID;
        } else {
            response["status"] = "error";
            response["message"] = "Incorrect password.";
        }
    } else {
        userData[login] = password;
        saveUserData();
        QString newSessionID = QUuid::createUuid().toString(QUuid::WithoutBraces);
        sessionData[newSessionID] = login;
        response["status"] = "success";
        response["message"] = "Registration successful.";
        response["sessionID"] = newSessionID;
    }

    clientSocket->write(QJsonDocument(response).toJson());
}

void Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        qDebug() << "Client disconnected";
        clients.remove(clientSocket->socketDescriptor());
        clientSocket->deleteLater();
    }
}

void Server::loadUserData()
{
    QFile file("users.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            userData = doc.object();
        }
    }
}

void Server::saveUserData()
{
    QFile file("users.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(userData);
        file.write(doc.toJson());
    }
}
