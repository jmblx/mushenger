#include "server.h"
#include <QDebug>
#include <QJsonDocument>
#include <QUuid>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <QDir>

// Конструктор
Server::Server(QObject *parent) : QTcpServer(parent)
{
    loadUserData();
    loadChatData();
}

// Метод для старта сервера
void Server::startServer(int port)
{
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Failed to start server on port" << port << ":" << this->errorString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (!clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Не удалось установить дескриптор сокета:" << clientSocket->errorString();
        clientSocket->deleteLater();
        return;
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::receiveData);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    qDebug() << "Новый клиент подключён с дескриптором:" << socketDescriptor;
}

void Server::sendErrorResponse(QTcpSocket *socket, const QString &message)
{
    QJsonObject response;
    response["status"] = "error";
    response["message"] = message;
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");

    qDebug() << "Sent error response:" << message;
}

void Server::receiveData()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (!clientSocket) return;

    qintptr handle = clientSocket->socketDescriptor();
    ClientState state = clientStates.value(handle, ClientState::Idle);

    QByteArray data = clientSocket->readAll();
    qDebug() << "Received data from handle" << handle << ":" << data;

    // Добавляем данные в буфер клиента
    clientBuffers[handle].append(data);

    // Обрабатываем все сообщения, разделённые '\n'
    while (clientBuffers[handle].contains('\n')) {
        int delimiterIndex = clientBuffers[handle].indexOf('\n');
        QByteArray singleMessage = clientBuffers[handle].left(delimiterIndex);
        clientBuffers[handle] = clientBuffers[handle].mid(delimiterIndex + 1);

        if (state == ClientState::Idle) {
            QJsonParseError parseError;
            QJsonDocument requestDoc = QJsonDocument::fromJson(singleMessage, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                sendErrorResponse(clientSocket, "Invalid JSON format.");
                qDebug() << "JSON parse error:" << parseError.errorString();
                continue;
            }

            QJsonObject request = requestDoc.object();
            QString requestType = request.value("type").toString();

            qDebug() << "Processing request type:" << requestType;

            // Обработка запроса
            if (requestType == "createChat") {
                handleCreateChat(clientSocket, request);
            }
            else if (requestType == "uploadAvatar") {
                handleUploadAvatar(clientSocket, request);
            }
            else if (requestType == "uploadProfileAvatar") {
                handleUploadProfileAvatar(clientSocket, request);
            }
            else if (requestType == "checkUser") {
                handleCheckUser(clientSocket, request);
            }
            else if (requestType == "message") {
                handleMessageRequest(clientSocket, request);
            }
            else if (requestType == "login" || requestType == "register") {
                handleLoginOrRegister(clientSocket, request);
            }
            else if (requestType == "getUserData") {
                handleGetUserData(clientSocket, request);
            }
            else if (requestType == "chatList") {
                handleChatListRequest(clientSocket, request);
            }
            else if (requestType == "getAvatar") {
                handleGetAvatar(clientSocket, request);
            }
            else if (requestType == "loadMessages") {
                handleLoadMessages(clientSocket, request);
            }
            else if (requestType == "syncMessages") {
                handleSyncMessages(clientSocket, request);
            }
            else if (requestType == "logout") {
                handleLogout(clientSocket, request);
            }
            else {
                sendErrorResponse(clientSocket, "Unknown request type.");
                qDebug() << "Unknown request type received from handle" << handle;
            }
        }
        else if (state == ClientState::ExpectingAvatar) {
            // Существующая обработка аватарных данных
            // Предположим, что данные аватара отправляются целиком и заканчиваются '\n'
            QByteArray avatarData = singleMessage;
            clientBuffers[handle].remove(0, clientBuffers[handle].size()); // Очистка буфера

            QString pendingChatID = clientPendingChatID.value(handle, "");
            if (pendingChatID.isEmpty()) {
                sendErrorResponse(clientSocket, "No chat ID specified for avatar upload.");
                qDebug() << "No chat ID specified for avatar upload from handle" << handle;
                clientStates[handle] = ClientState::Idle;
                return;
            }

            // Сохранение аватара
            QDir dir;
            if (!dir.exists("chat_avs")) {
                if (!dir.mkpath("chat_avs")) {
                    sendErrorResponse(clientSocket, "Failed to create avatar directory.");
                    qDebug() << "Failed to create 'chat_avs' directory.";
                    clientStates[handle] = ClientState::Idle;
                    clientPendingChatID.remove(handle);
                    return;
                }
            }

            QString avatarPath = QString("chat_avs/%1.png").arg(pendingChatID);

            QFile file(avatarPath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(avatarData);
                file.close();
                qDebug() << "Avatar saved to" << avatarPath << "for chatID" << pendingChatID;
            }
            else {
                sendErrorResponse(clientSocket, "Failed to save avatar.");
                qDebug() << "Failed to save avatar for chatID" << pendingChatID << "from handle" << handle;
                clientStates[handle] = ClientState::Idle;
                clientPendingChatID.remove(handle);
                return;
            }

            // Обновление данных чата с путем к аватару
            QJsonObject chatObj = chatData[pendingChatID];
            chatObj["avatarPath"] = avatarPath;
            chatData[pendingChatID] = chatObj;
            saveChatData();

            // Отправка успешного ответа клиенту
            QJsonObject response;
            response["status"] = "success";
            response["message"] = "Avatar uploaded successfully.";
            clientSocket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n"); // Добавляем '\n'
            clientSocket->flush();

            qDebug() << "Avatar uploaded successfully for chatID" << pendingChatID << "from handle" << handle;

            // Сброс состояния клиента
            clientStates[handle] = ClientState::Idle;
            clientPendingChatID.remove(handle);
        }
        else {
            // Неизвестное состояние
            sendErrorResponse(clientSocket, "Unknown client state.");
            qDebug() << "Unknown client state for handle" << handle;
            clientStates[handle] = ClientState::Idle;
            clientPendingChatID.remove(handle);
            clientBuffers.remove(handle);
        }
    }
}

void Server::clientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QString loginToRemove;
        for (auto it = onlineClients.begin(); it != onlineClients.end(); ++it) {
            if (it.value() == clientSocket) {
                loginToRemove = it.key();
                break;
            }
        }

        if (!loginToRemove.isEmpty()) {
            onlineClients.remove(loginToRemove);
            qDebug() << "User" << loginToRemove << "is now offline.";
        }
        qintptr handle = clientSocket->socketDescriptor();
        qDebug() << "Client disconnected with handle" << handle;

        clientStates.remove(handle);
        clientPendingChatName.remove(handle);
        clientPendingChatID.remove(handle);
        clientBuffers.remove(handle);

        clientSocket->deleteLater();
    }
}

void Server::handleCreateChat(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        QJsonObject response;
        response["status"] = "error";
        response["message"] = "Invalid session.";
        socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
        socket->flush();
        qDebug() << "Invalid session ID during chat creation.";
        return;
    }

    QString login = sessionData[sessionID];
    QString chatName = request.value("chatName").toString();
    QJsonArray participantsArray = request.value("participants").toArray();

    // Удаление дубликатов участников
    QSet<QString> uniqueParticipantsSet;
    for (const QJsonValue &participant : participantsArray) {
        uniqueParticipantsSet.insert(participant.toString());
    }

    // Преобразование обратно в QJsonArray
    QJsonArray uniqueParticipantsArray;
    for (const QString &participant : uniqueParticipantsSet) {
        uniqueParticipantsArray.append(participant);
    }

    // Проверка существования всех участников
    bool allUsersExist = true;
    QString nonExistentUser;
    for (const QJsonValue &participant : uniqueParticipantsArray) {
        QString username = participant.toString();
        if (!userData.contains(username)) {
            allUsersExist = false;
            nonExistentUser = username;
            break;
        }
    }

    if (!allUsersExist) {
        QJsonObject response;
        response["status"] = "error";
        response["message"] = QString("User '%1' does not exist.").arg(nonExistentUser);
        socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
        socket->flush();
        qDebug() << "Attempted to create chat with non-existent user:" << nonExistentUser;
        return;
    }

    // Определение типа чата
    bool isGroupChat = uniqueParticipantsArray.size() > 2;

    // Создание нового чата
    QString chatID = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QJsonObject newChat;
    newChat["id"] = chatID;
    newChat["name"] = chatName;
    newChat["avatarPath"] = "";
    newChat["participants"] = uniqueParticipantsArray;
    newChat["messages"] = QJsonArray();
    newChat["isGroup"] = isGroupChat;

    chatData[chatID] = newChat;
    saveChatData();

    // Добавление чата в список чатов каждого участника
    for (const QString &participant : uniqueParticipantsSet) {
        if (userData.contains(participant)) {
            QJsonObject userObj = userData[participant];
            QJsonArray userChats = userObj["chats"].toArray();
            userChats.append(chatID);
            userObj["chats"] = userChats;
            userData[participant] = userObj;
        }
    }
    saveUserData();

    QJsonObject response;
    response["status"] = "success";
    response["chatID"] = chatID; // Добавляем chatID в ответ
    response["message"] = "Chat created successfully.";
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
    qDebug() << "Chat created successfully with ID:" << chatID;
}

// Обработка запроса проверки пользователя
void Server::handleCheckUser(QTcpSocket *socket, const QJsonObject &request)
{
    QString username = request.value("username").toString();

    QJsonObject response;
    response["type"] = "checkUser"; // Добавляем поле type
    response["username"] = username; // Добавляем username в ответ

    if (userData.contains(username)) {
        response["status"] = "success";
        response["message"] = "User exists.";
    }
    else {
        response["status"] = "error";
        response["message"] = "User does not exist.";
    }
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Check user request for username:" << username << "- Status:" << response["status"].toString();
}

void Server::handleMessageRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString chatID = request.value("chatID").toString();
    QString messageText = request.value("message").toString();
    QString sessionID = request.value("sessionID").toString();
    QString sender = sessionData.value(sessionID, "");

    if (sender.isEmpty()) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Invalid session ID during message sending.";
        return;
    }

    if (!chatData.contains(chatID)) {
        sendErrorResponse(socket, "Chat not found.");
        qDebug() << "Chat ID not found:" << chatID;
        return;
    }

    QJsonObject chatObj = chatData[chatID];

    // Проверка, является ли отправитель участником чата
    QJsonArray participants = chatObj["participants"].toArray();
    bool isParticipant = false;
    for (const QJsonValue &participant : participants) {
        if (participant.toString() == sender) {
            isParticipant = true;
            break;
        }
    }

    if (!isParticipant) {
        sendErrorResponse(socket, "You are not a participant of this chat.");
        qDebug() << "User" << sender << "is not a participant of chat" << chatID;
        return;
    }

    QJsonObject message;
    message["sender"] = sender;
    message["text"] = messageText;
    message["timestamp"] = QDateTime::currentDateTimeUtc().toSecsSinceEpoch(); // Используем UTC

    QJsonArray chatMessages = chatObj["messages"].toArray();
    chatMessages.append(message);
    chatObj["messages"] = chatMessages;
    chatData[chatID] = chatObj;
    saveChatData();

    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Message sent.";
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    // Отправка нового сообщения всем онлайн-участникам чата кроме отправителя
    for (const QJsonValue &participantVal : chatObj["participants"].toArray()) {
        QString participant = participantVal.toString();
        if (onlineClients.contains(participant) && participant != sender) {
            QTcpSocket *participantSocket = onlineClients[participant];
            QJsonObject msgToSend = message;

            QJsonObject msgResponse;
            msgResponse["status"] = "success";

            // Формирование newMessages
            QJsonArray newMessagesArray;
            newMessagesArray.append(msgToSend);

            QJsonObject newMessagesObj;
            newMessagesObj.insert(chatID, newMessagesArray);

            msgResponse["newMessages"] = newMessagesObj;
            msgResponse["type"] = "newMessages";

            participantSocket->write(QJsonDocument(msgResponse).toJson(QJsonDocument::Compact) + "\n");
            participantSocket->flush();

            qDebug() << "Sent new message to online user" << participant << ":" << QJsonDocument(msgResponse).toJson(QJsonDocument::Compact);
        }
    }

    qDebug() << "Message sent in chat" << chatID << "by user" << sender;
}


void Server::handleSyncMessages(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    QJsonObject lastMessages = request.value("lastMessages").toObject(); // Ключ: chatID, значение: timestamp

    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Invalid session ID during syncMessages.";
        return;
    }

    QString userLogin = sessionData[sessionID];
    QJsonObject newMessages;

    // Проходим по всем чатам, в которых пользователь участвует
    for (auto it = chatData.begin(); it != chatData.end(); ++it) {
        QString chatID = it.key();
        QJsonObject chatObj = it.value();

        // Проверяем, является ли пользователь участником чата
        QJsonArray participants = chatObj["participants"].toArray();
        bool isParticipant = false;
        for (const QJsonValue &participant : participants) {
            if (participant.toString() == userLogin) {
                isParticipant = true;
                break;
            }
        }

        if (!isParticipant) {
            continue;
        }

        QJsonArray messages = chatObj["messages"].toArray();
        QJsonArray newMessagesArray;

        // Получаем последний timestamp для этого чата от клиента
        qint64 lastTimestamp = 0;
        if (lastMessages.contains(chatID)) {
            lastTimestamp = lastMessages[chatID].toInt();
        }

        // Находим все сообщения после последнего timestamp
        for (const QJsonValue &msgVal : messages) {
            QJsonObject msgObj = msgVal.toObject();
            qint64 msgTimestamp = msgObj.value("timestamp").toInt();
            if (msgTimestamp > lastTimestamp) {
                newMessagesArray.append(msgObj);
            }
        }

        if (!newMessagesArray.isEmpty()) {
            newMessages.insert(chatID, newMessagesArray);
        }
    }

    QJsonObject response;
    response["status"] = "success";
    response["newMessages"] = newMessages;

    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Sent new messages to handle" << socket->socketDescriptor();
}

// Обработка запроса списка чатов
void Server::handleChatListRequest(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();

    if (!sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Invalid session ID during chat list request.";
        return;
    }

    QString login = sessionData[sessionID];
    QJsonArray userChats = userData[login]["chats"].toArray();
    QJsonArray chatList;

    for (const QJsonValue &chatID : userChats) {
        QString chatIDStr = chatID.toString();
        if (chatData.contains(chatIDStr)) {
            QJsonObject chat = chatData[chatIDStr];
            QJsonObject chatInfo;
            chatInfo["id"] = chatIDStr;
            chatInfo["name"] = chat["name"].toString();
            chatInfo["avatarPath"] = chat["avatarPath"].toString();
            chatInfo["isGroup"] = chat["isGroup"].toBool();
            chatInfo["participants"] = chat["participants"].toArray();
            chatList.append(chatInfo);
        }
    }

    QJsonObject response;
    response["status"] = "success";
    response["type"] = "chatList";
    response["chats"] = chatList;

    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Chat list sent to handle" << socket->socketDescriptor();
}

// Обработка запроса аватара
void Server::handleGetAvatar(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    QString chatID = request.value("chatID").toString();

    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(clientSocket, "Invalid session.");
        qDebug() << "Invalid session ID during getAvatar.";
        return;
    }

    if (!chatData.contains(chatID)) {
        sendErrorResponse(clientSocket, "Chat not found.");
        qDebug() << "Chat ID not found during getAvatar:" << chatID;
        return;
    }

    QJsonObject chatObj = chatData[chatID];
    QString avatarPath = chatObj["avatarPath"].toString();

    if (avatarPath.isEmpty()) {
        sendErrorResponse(clientSocket, "No avatar for this chat.");
        qDebug() << "No avatar set for chat" << chatID;
        return;
    }

    QFile avatarFile(avatarPath);
    if (!avatarFile.exists()) {
        sendErrorResponse(clientSocket, "Avatar file does not exist.");
        qDebug() << "Avatar file does not exist at path:" << avatarPath;
        return;
    }

    if (!avatarFile.open(QIODevice::ReadOnly)) {
        sendErrorResponse(clientSocket, "Failed to open avatar file.");
        qDebug() << "Failed to open avatar file at path:" << avatarPath;
        return;
    }

    QByteArray avatarData = avatarFile.readAll();
    avatarFile.close();

    // Кодируем аватар в Base64
    QByteArray avatarBase64 = avatarData.toBase64();

    // Отправляем JSON-ответ с аватаром
    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Avatar data.";
    response["chatID"] = chatID;
    response["avatar"] = QString(avatarBase64);

    clientSocket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    clientSocket->flush();

    qDebug() << "Avatar sent to client for chat" << chatID;
}

void Server::handleUploadAvatar(QTcpSocket *socket, const QJsonObject &request)
{
    QString chatID = request.value("chatID").toString();
    QString sessionID = request.value("sessionID").toString();
    QString avatarBase64 = request.value("avatar").toString();

    if (chatID.isEmpty() || sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session or chat ID.");
        qDebug() << "Invalid session ID or chat ID in uploadAvatar from handle" << socket->socketDescriptor();
        return;
    }

    if (!chatData.contains(chatID)) {
        sendErrorResponse(socket, "Chat does not exist.");
        qDebug() << "Chat ID does not exist in uploadAvatar:" << chatID;
        return;
    }

    // Декодирование Base64
    QByteArray avatarData = QByteArray::fromBase64(avatarBase64.toUtf8());

    // Проверка корректности данных аватара
    QImage image;
    if (!image.loadFromData(avatarData, "PNG")) {
        sendErrorResponse(socket, "Invalid avatar data.");
        qDebug() << "Invalid avatar data received for chatID:" << chatID;
        return;
    }

    // Сохранение аватара
    QDir dir;
    if (!dir.exists("chat_avs")) {
        if (!dir.mkpath("chat_avs")) {
            sendErrorResponse(socket, "Failed to create avatar directory.");
            qDebug() << "Failed to create 'chat_avs' directory.";
            return;
        }
    }

    QString avatarPath = QString("chat_avs/%1.png").arg(chatID);

    QFile file(avatarPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(avatarData);
        file.close();
        qDebug() << "Avatar saved to" << avatarPath << "for chatID" << chatID;
    }
    else {
        sendErrorResponse(socket, "Failed to save avatar.");
        qDebug() << "Failed to save avatar for chatID" << chatID;
        return;
    }

    // Обновление данных чата с путем к аватару
    QJsonObject chatObj = chatData[chatID];
    chatObj["avatarPath"] = avatarPath;
    chatData[chatID] = chatObj;
    saveChatData();

    // Отправка успешного ответа клиенту
    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Avatar uploaded successfully.";
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Avatar uploaded successfully for chatID" << chatID << "from handle" << socket->socketDescriptor();
}

// Обработка запроса получения данных пользователя
void Server::handleGetUserData(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    qDebug() << "Received session ID from client:" << sessionID;

    if (!sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Session not found:" << sessionID;
        return;
    }

    QString login = sessionData[sessionID];
    qDebug() << "Session found, user login:" << login;

    if (!userData.contains(login)) {
        sendErrorResponse(socket, "User not found.");
        qDebug() << "User not found for login:" << login;
        return;
    }

    QJsonObject userObject = userData[login];

    // Читаем аватарку и кодируем в Base64
    QString avatarPath = userObject.value("avatarPath").toString();
    QString avatarBase64;
    if (!avatarPath.isEmpty()) {
        QFile avatarFile(avatarPath);
        if (avatarFile.open(QIODevice::ReadOnly)) {
            QByteArray avatarData = avatarFile.readAll();
            avatarBase64 = QString(avatarData.toBase64());
            avatarFile.close();
        }
    }
    userObject["avatar"] = avatarBase64;

    QJsonObject response;
    response["status"] = "success";
    response["login"] = login;
    response["user"] = userObject;
    response["sessionID"] = sessionID;
    response["type"] = "userData";

    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
}

// Обработка загрузки аватара для профиля
void Server::handleUploadProfileAvatar(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    QString avatarBase64 = request.value("avatar").toString();

    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session ID.");
        qDebug() << "Invalid session ID in uploadProfileAvatar from handle" << socket->socketDescriptor();
        return;
    }

    QString login = sessionData[sessionID];

    // Декодирование Base64
    QByteArray avatarData = QByteArray::fromBase64(avatarBase64.toUtf8());

    // Проверка корректности данных аватарки
    QImage image;
    if (!image.loadFromData(avatarData, "PNG")) {
        sendErrorResponse(socket, "Invalid avatar data.");
        qDebug() << "Invalid avatar data received for sessionID:" << sessionID;
        return;
    }

    // Сохранение аватарки
    QDir dir;
    if (!dir.exists("profile_avs")) {
        if (!dir.mkpath("profile_avs")) {
            sendErrorResponse(socket, "Failed to create avatar directory.");
            qDebug() << "Failed to create 'profile_avs' directory.";
            return;
        }
    }

    QString avatarPath = QString("profile_avs/%1.png").arg(login);

    if (!image.save(avatarPath)) {
        sendErrorResponse(socket, "Failed to save avatar.");
        qDebug() << "Failed to save avatar for login" << login;
        return;
    }

    // Обновление данных пользователя с путем к аватарке
    if (userData.contains(login)) {
        QJsonObject userObj = userData[login];
        userObj["avatarPath"] = avatarPath;
        userData[login] = userObj;
        saveUserData();
    }

    // Отправка успешного ответа клиенту
    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Avatar uploaded successfully.";
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Avatar uploaded successfully for login" << login << "from handle" << socket->socketDescriptor();
}

// Метод для загрузки данных пользователей из файла
void Server::loadUserData()
{
    QFile file("users.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                if (it.value().isObject()) { // Проверка, что значение является объектом
                    userData.insert(it.key(), it.value().toObject());
                }
            }
            qDebug() << "User data loaded successfully.";
        }
        else {
            qDebug() << "Failed to parse user data JSON.";
        }
        file.close();
    }
    else {
        qDebug() << "Failed to open users.json. Starting with empty user data.";
    }
}

// Метод для сохранения данных пользователей в файл
void Server::saveUserData()
{
    QFile file("users.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        for (auto it = userData.begin(); it != userData.end(); ++it) {
            obj.insert(it.key(), it.value());
        }
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
        qDebug() << "User data saved successfully.";
    }
    else {
        qDebug() << "Failed to save user data.";
    }
}

// Метод для загрузки данных чатов из файла
void Server::loadChatData()
{
    QFile file("chats.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                if (it.value().isObject()) { // Проверка, что значение является объектом
                    chatData.insert(it.key(), it.value().toObject());
                }
            }
            qDebug() << "Chat data loaded successfully.";
        }
        else {
            qDebug() << "Failed to parse chat data JSON.";
        }
        file.close();
    }
    else {
        qDebug() << "Failed to open chats.json. Starting with empty chat data.";
    }
}

// Метод для сохранения данных чатов в файл
void Server::saveChatData()
{
    QFile file("chats.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        for (auto it = chatData.begin(); it != chatData.end(); ++it) {
            obj.insert(it.key(), it.value());
        }
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
        qDebug() << "Chat data saved successfully.";
    }
    else {
        qDebug() << "Failed to save chat data.";
    }
}

// Обработка логина и регистрации
void Server::handleLoginOrRegister(QTcpSocket *socket, const QJsonObject &request)
{
    QString login = request.value("login").toString();
    QString password = request.value("password").toString();

    QJsonObject response;

    // Если пользователь существует, проверяем пароль
    if (userData.contains(login)) {
        if (userData[login]["password"].toString() == password) {
            // Логин успешен, создаём сессию
            QString newSessionID = QUuid::createUuid().toString(QUuid::WithoutBraces);
            sessionData[newSessionID] = login;  // Сохраняем сессию с логином

            response["status"] = "success";
            response["message"] = "Login successful.";
            response["sessionID"] = newSessionID; // Передаём новую сессию клиенту
            response["login"] = login; // Передаем логин пользователя
            qDebug() << "User" << login << "logged in successfully with session ID" << newSessionID;

            // Добавляем клиента в онлайн
            onlineClients.insert(login, socket);
        }
        else {
            response["status"] = "error";
            response["message"] = "Incorrect password.";
            response["type"] = "loginOrRegister";
            qDebug() << "Incorrect password attempt for user" << login;
        }
    }
    else {
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
        response["login"] = login;
        response["type"] = "loginOrRegister";
        qDebug() << "User" << login << "registered successfully with session ID" << newSessionID;

        // Добавляем клиента в онлайн
        onlineClients.insert(login, socket);
    }

    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
}


void Server::handleLoadMessages(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();
    QString chatID = request.value("chatID").toString();

    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Invalid session ID during loadMessages.";
        return;
    }

    QString username = sessionData[sessionID];

    if (!chatData.contains(chatID)) {
        sendErrorResponse(socket, "Chat not found.");
        qDebug() << "Chat ID not found during loadMessages:" << chatID;
        return;
    }

    QJsonObject chatObj = chatData[chatID];

    // Проверка, является ли пользователь участником чата
    QJsonArray participants = chatObj["participants"].toArray();
    bool isParticipant = false;
    for (const QJsonValue &participant : participants) {
        if (participant.toString() == username) {
            isParticipant = true;
            break;
        }
    }

    if (!isParticipant) {
        sendErrorResponse(socket, "You are not a participant of this chat.");
        qDebug() << "User" << username << "is not a participant of chat" << chatID;
        return;
    }

    QJsonArray messages = chatObj["messages"].toArray();

    QJsonObject response;
    response["status"] = "success";
    response["messages"] = messages;
    response["type"] = "loadMessages";

    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n"); // Добавляем '\n' как разделитель
    socket->flush();

    qDebug() << "Messages sent to user" << username << "for chat" << chatID;
}


void Server::handleLogout(QTcpSocket *socket, const QJsonObject &request)
{
    QString sessionID = request.value("sessionID").toString();

    if (sessionID.isEmpty() || !sessionData.contains(sessionID)) {
        sendErrorResponse(socket, "Invalid session.");
        qDebug() << "Invalid session ID during logout.";
        return;
    }

    QString login = sessionData[sessionID];
    qDebug() << "User" << login << "logged out.";

    // Удаление сессии
    sessionData.remove(sessionID);

    // Удаление клиента из списка онлайн
    onlineClients.remove(login);
    qDebug() << "Session removed, client logged out.";

    // Отправка ответа клиенту об успешном выходе
    QJsonObject response;
    response["status"] = "success";
    response["message"] = "Logout successful.";
    socket->write(QJsonDocument(response).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Logout response sent to client.";
}
