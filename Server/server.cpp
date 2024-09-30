#include "server.h"
#include <QDebug>
#include <QJsonDocument>
#include <QUuid>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    loadUserData();
    loadChatData();
}

void Server::startServer(int port)
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

void Server::handleChatRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString login = sessionData[request.value("sessionID").toString()];

    // Загрузка списка чатов пользователя
    QJsonArray userChats = userData[login].toObject()["chats"].toArray();
    QJsonArray chatList;

    for (const QJsonValue &chatID : userChats) {
        QJsonObject chat = chatData[chatID.toString()].toObject();
        chatList.append(chat);
    }

    QJsonObject response;
    response["status"] = "success";
    response["chats"] = chatList;

    socket->write(QJsonDocument(response).toJson());
}

void Server::handleCreateChat(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        QJsonObject response;
        response["status"] = "error";
        response["message"] = "Invalid session.";
        socket->write(QJsonDocument(response).toJson());
        return;
    }

    QString login = sessionData[sessionID];
    QString chatName = request.value("chatName").toString();
    QString chatID = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QJsonArray participants = request.value("participants").toArray();

    // Проверка на личный или групповой чат
    bool isGroupChat = participants.size() > 2;

    QJsonObject newChat;
    newChat["name"] = chatName;
    newChat["avatarPath"] = "";
    newChat["participants"] = participants;
    newChat["messages"] = QJsonArray();
    newChat["isGroup"] = isGroupChat;

    chatData[chatID] = newChat;
    saveChatData();

    // Добавляем чат пользователю
    for (const auto &participant : participants) {
        QString user = participant.toString();
        if (userData.contains(user)) {
            QJsonObject userObj = userData[user].toObject();
            QJsonArray userChats = userObj["chats"].toArray();
            userChats.append(chatID);
            userObj["chats"] = userChats;
            userData[user] = userObj;
        }
    }
    saveUserData();

    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Chat created successfully.";
    response["chatID"] = chatID;
    socket->write(QJsonDocument(response).toJson());
}

void Server::handleAvatarUpload(QTcpSocket *socket, const QJsonObject &request, const QByteArray &data)
{
    QString sessionID = request.value("sessionID").toString();
    QString chatName = request.value("chatName").toString();

    if (!sessionData.contains(sessionID)) {
        QJsonObject response;
        response["status"] = "error";
        response["message"] = "Invalid session.";
        socket->write(QJsonDocument(response).toJson());
        return;
    }

    // Извлечение данных аватарки из байт после JSON запроса
    QByteArray avatarData = data.mid(request.size());

    QString avatarPath = QString("avatars/%1.png").arg(chatName);
    QFile file(avatarPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(avatarData);
        file.close();
    } else {
        QJsonObject response;
        response["status"] = "error";
        response["message"] = "Failed to save avatar.";
        socket->write(QJsonDocument(response).toJson());
        return;
    }

    // Корректное изменение chatData
    for (auto chatKey : chatData.keys()) {
        QJsonObject chatObj = chatData[chatKey].toObject();
        if (chatObj["name"].toString() == chatName) {
            chatObj["avatarPath"] = avatarPath;
            chatData[chatKey] = chatObj; // Сохранение измененного объекта обратно в chatData
            saveChatData();
            break;
        }
    }

    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Avatar uploaded successfully.";
    socket->write(QJsonDocument(response).toJson());
}

void Server::handleCheckUser(QTcpSocket *socket, const QJsonObject &request)
{
    QString username = request.value("username").toString();

    QJsonObject response;
    if (userData.contains(username)) {
        response["status"] = "success";
        response["message"] = "User exists.";
    } else {
        response["status"] = "error";
        response["message"] = "User does not exist.";
    }
    socket->write(QJsonDocument(response).toJson());
}



void Server::handleMessageRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString chatID = request.value("chatID").toString();
    QString messageText = request.value("message").toString();
    QString sender = sessionData[request.value("sessionID").toString()];

    QJsonObject message;
    message["sender"] = sender;
    message["text"] = messageText;

    QJsonArray chatMessages = chatData[chatID].toObject()["messages"].toArray();
    chatMessages.append(message);
    chatData[chatID].toObject()["messages"] = chatMessages;
    saveChatData();

    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Message sent.";
    socket->write(QJsonDocument(response).toJson());
}


void Server::receiveData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QJsonDocument requestDoc = QJsonDocument::fromJson(data);
    if (!requestDoc.isObject()) {
        sendErrorResponse(clientSocket, "Invalid JSON format.");
        return;
    }

    QJsonObject request = requestDoc.object();
    QString requestType = request.value("type").toString();

    if (requestType == "createChat") {
        handleCreateChat(clientSocket, request);
    } else if (requestType == "uploadAvatar") {
        handleAvatarUpload(clientSocket, request, data);
    } else if (requestType == "checkUser") {
        handleCheckUser(clientSocket, request);
    } else if (requestType == "message") {
        handleMessageRequest(clientSocket, request);
    } else if (requestType == "login" || requestType == "register") {
        handleLoginOrRegister(clientSocket, request);
    } else if (requestType == "getUserData") {
        handleGetUserData(clientSocket, request);
    } else if (requestType == "chatList") {
        handleChatListRequest(clientSocket, request);
    } else {
        sendErrorResponse(clientSocket, "Unknown request type.");
    }
}

void Server::handleGetUserData(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();

    // Проверка наличия сессии в sessionData
    if (!sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Сессия не найдена:" << sessionID; // Добавлено для отладки
        return;
    }

    QString login = sessionData[sessionID];  // Получаем логин пользователя по сессии
    qDebug() << "Сессия найдена, логин пользователя:" << login; // Добавлено для отладки

    if (!userData.contains(login)) {
        sendErrorResponse(socket, "User not found.");
        qDebug() << "Пользователь не найден для логина:" << login; // Добавлено для отладки
        return;
    }

    QJsonObject userObject = userData[login].toObject();
    QJsonObject response;
    response["status"] = "success";
    response["login"] = login;  // Возвращаем логин пользователя
    response["user"] = userObject;

    qDebug() << "Отправка данных пользователя для логина:" << login; // Добавлено для отладки
    socket->write(QJsonDocument(response).toJson());
}

void Server::handleChatListRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();

    if (!sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        return;
    }

    QString login = sessionData[sessionID];
    QJsonArray userChats = userData[login].toObject()["chats"].toArray();
    QJsonArray chatList;

    for (const QJsonValue &chatID : userChats) {
        QJsonObject chat = chatData[chatID.toString()].toObject();
        chatList.append(chat);
    }

    QJsonObject response;
    response["status"] = "success";
    response["chats"] = chatList;

    socket->write(QJsonDocument(response).toJson());
}

void Server::sendErrorResponse(QTcpSocket *socket, const QString &message)
{
    QJsonObject response;
    response["status"] = "error";
    response["message"] = message;
    socket->write(QJsonDocument(response).toJson());
}

void Server::handleLoginOrRegister(QTcpSocket *socket, const QJsonObject &request)
{
    QString login = request.value("login").toString();
    QString password = request.value("password").toString();

    QJsonObject response;

    // Если пользователь существует, проверяем пароль
    if (userData.contains(login)) {
        if (userData[login].toObject()["password"].toString() == password) {
            // Логин успешен, создаём сессию
            QString newSessionID = QUuid::createUuid().toString(QUuid::WithoutBraces);
            sessionData[newSessionID] = login;  // Сохраняем сессию с логином

            response["status"] = "success";
            response["message"] = "Login successful.";
            response["sessionID"] = newSessionID; // Передаём новую сессию клиенту
            response["login"] = login; // Передаем логин пользователя
        } else {
            response["status"] = "error";
            response["message"] = "Incorrect password.";
        }
    } else {
        // Пользователь не существует, выполняем регистрацию
        QJsonObject newUser;
        newUser["password"] = password;
        newUser["chats"] = QJsonArray(); // Пустой массив чатов для нового пользователя
        userData[login] = newUser;
        saveUserData();

        // Создаём новую сессию
        QString newSessionID = QUuid::createUuid().toString(QUuid::WithoutBraces);
        sessionData[newSessionID] = login;

        response["status"] = "success";
        response["message"] = "Registration successful.";
        response["sessionID"] = newSessionID; // Передаём новую сессию клиенту
        response["login"] = login; // Передаем логин пользователя
    }

    socket->write(QJsonDocument(response).toJson());
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

void Server::loadChatData()
{
    QFile file("chats.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            chatData = doc.object();
        }
    }
}

void Server::saveChatData()
{
    QFile file("chats.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(chatData);
        file.write(doc.toJson());
    }
}
