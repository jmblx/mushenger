#include "ChatScreen.h"
#include "ui_ChatScreen.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include <QListWidgetItem>

// Конструктор
ChatScreen::ChatScreen(const QString &sessionID, const QString &userLogin, QTcpSocket *existingSocket, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatScreen), socket(existingSocket),
    sessionID(sessionID), currentUserLogin(userLogin)
{
    ui->setupUi(this);

    // Подключение сигналов и слотов
    connect(ui->chatsList, &QListWidget::itemClicked, this, &ChatScreen::onChatSelected);
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatScreen::onSendMessageClicked);
    connect(socket, &QTcpSocket::readyRead, this, &ChatScreen::onReadyRead);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &ChatScreen::onEnterPressed);
    connect(ui->newChat, &QPushButton::clicked, this, &ChatScreen::onNewChatClicked);
    connect(this, &ChatScreen::userCheckCompleted, this, &ChatScreen::onUserCheckCompleted); // Новый сигнал

    ui->userName->setText(currentUserLogin);

    loadLocalCache();

    loadChats();
}

// Деструктор
ChatScreen::~ChatScreen()
{
    delete ui;
}

void ChatScreen::sendRequest(const QJsonObject &request)
{
    QJsonDocument doc(request);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n"; // Компактный формат JSON
    qDebug() << "Отправка запроса на сервер:" << jsonData;

    socket->write(jsonData);
    socket->flush();
}

// Метод для загрузки чатов с сервера
void ChatScreen::loadChats()
{
    QJsonObject request;
    request["type"] = "chatList";
    request["sessionID"] = sessionID;
    sendRequest(request);
}

void ChatScreen::loadMessages(const QString &chatID)
{
    currentChatID = chatID;
    QJsonObject request;
    request["type"] = "loadMessages"; // Убедитесь, что сервер обрабатывает этот тип запроса
    request["sessionID"] = sessionID;
    request["chatID"] = chatID;
    sendRequest(request);
}

void ChatScreen::onChatSelected(QListWidgetItem *item)
{
    QString chatID = item->data(Qt::UserRole).toString();
    currentChatID = chatID;

    qDebug() << "Выбран чат с ID:" << chatID;

    // Загрузка сообщений из локального кэша
    if (localMessageCache.contains(chatID)) {
        QJsonArray localMessages = localMessageCache[chatID].value("messages").toArray();
        ui->messagesList->clear();
        displayMessages(localMessages);
    }

    // Загрузка новых сообщений с сервера
    loadMessages(chatID);
}

void ChatScreen::onSendMessageClicked()
{
    QString messageText = ui->messageInput->text();
    if (messageText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сообщение не может быть пустым.");
        return;
    }

    if (currentChatID.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите чат для отправки сообщения.");
        return;
    }

    QJsonObject request;
    request["type"] = "message";
    request["sessionID"] = sessionID;
    request["chatID"] = currentChatID;
    request["message"] = messageText;
    sendRequest(request);
    ui->messageInput->clear();

    // Добавить сообщение в UI сразу
    QString formattedMessage = currentUserLogin + ": " + messageText;
    ui->messagesList->addItem(new QListWidgetItem(formattedMessage));
    ui->messagesList->scrollToBottom();

    qDebug() << "Добавлено сообщение отправителя в UI:" << formattedMessage;

    // Добавить сообщение в локальный кэш
    if (!localMessageCache.contains(currentChatID)) {
        QJsonObject chatCache;
        chatCache["messages"] = QJsonArray();
        localMessageCache.insert(currentChatID, chatCache);
    }

    QJsonArray existingMessages = localMessageCache[currentChatID].value("messages").toArray();
    QJsonObject newMessage;
    newMessage["sender"] = currentUserLogin;
    newMessage["text"] = messageText;
    newMessage["timestamp"] = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    existingMessages.append(newMessage);
    localMessageCache[currentChatID]["messages"] = existingMessages;

    saveLocalCache();
    qDebug() << "Сообщение добавлено в локальный кэш и сохранено.";
}

QByteArray ChatScreen::processAvatar(const QString &filePath)
{
    QImage image(filePath);
    if (image.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение.");
        return QByteArray();
    }

    QImage scaledImage = image.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    scaledImage.save(&buffer, "PNG");

    return imageData;
}

// Слот для обработки нажатия Enter
void ChatScreen::onEnterPressed()
{
    if (!ui->messageInput->text().isEmpty()) {
        onSendMessageClicked();
    }
}

void ChatScreen::onNewChatClicked()
{
    chatName = QInputDialog::getText(this, "Создание нового чата", "Введите название чата:");
    if (chatName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название чата не может быть пустым.");
        return;
    }

    QString avatarPath = QFileDialog::getOpenFileName(this, "Выберите аватарку", "", "Images (*.png *.jpg *.bmp)");
    if (!avatarPath.isEmpty()) {
        avatarData = processAvatar(avatarPath);
        if (avatarData.isEmpty()) {
            return;
        }
    }
    else {
        avatarData.clear();
    }
    validParticipants.clear();
    validParticipants.insert(currentUserLogin);
    addParticipant();
}

void ChatScreen::addParticipant()
{
    QString newParticipant = QInputDialog::getText(this, "Добавление участника", "Введите логин участника:");
    if (newParticipant.isEmpty()) {
        // Нет больше участников, создаём чат
        createChat();
        return;
    }

    // Проверяем, не добавлен ли уже участник
    if (validParticipants.contains(newParticipant)) {
        QMessageBox::warning(this, "Предупреждение", "Этот пользователь уже добавлен в чат.");
        addParticipant(); // Повторяем попытку добавления
        return;
    }

    // Асинхронная проверка существования пользователя
    lastCheckedUsername = newParticipant;
    checkUserExists(newParticipant);
}
void ChatScreen::checkUserExists(const QString &username)
{
    QJsonObject request;
    request["type"] = "checkUser";
    request["sessionID"] = sessionID;
    request["username"] = username;

    sendRequest(request);
}

void ChatScreen::onUserCheckCompleted(const QString &username, bool exists)
{
    if (username != lastCheckedUsername) {
        qDebug() << "Получено имя пользователя не совпадает с ожидаемым.";
        return;
    }

    if (exists) {
        qDebug() << "Пользователь найден:" << username;
        validParticipants.insert(username);
        QMessageBox::information(this, "Успех", QString("Пользователь %1 добавлен.").arg(username));
    }
    else {
        qDebug() << "Пользователь не найден:" << username;
        QMessageBox::warning(this, "Ошибка", QString("Пользователь %1 не найден.").arg(username));
    }

    // Продолжаем добавлять следующих участников
    addParticipant();
}


void ChatScreen::createChat()
{
    QJsonObject request;
    request["type"] = "createChat";
    request["sessionID"] = sessionID;
    request["chatName"] = chatName;

    QStringList participantsList;
    for (const QString &participant : validParticipants) {
        participantsList << participant;
    }
    QJsonArray participantsArray = QJsonArray::fromStringList(participantsList);
    request["participants"] = participantsArray;

    sendRequest(request);

    if (!avatarData.isEmpty()) {
        // Сохраняем данные аватара для отправки
        pendingAvatarData = avatarData;
        pendingChatName = chatName;
    }
}

void ChatScreen::fetchChatAvatar(const QString &chatID)
{
    QJsonObject request;
    request["type"] = "getAvatar";
    request["sessionID"] = sessionID;
    request["chatID"] = chatID;

    sendRequest(request);
    qDebug() << "Отправлен запрос на получение аватара для chatID:" << chatID;
}

void ChatScreen::displayChatAvatar(const QString &chatID, const QImage &avatar)
{
    // Найдите элемент списка чатов по chatID и установите иконку
    for(int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        if(item->data(Qt::UserRole).toString() == chatID) {
            QIcon icon(QPixmap::fromImage(avatar));
            item->setIcon(icon);
            chatAvatars[chatID] = avatar; // Сохраняем аватар
            qDebug() << "Установлен аватар для chatID:" << chatID;
            break;
        }
    }
}

QListWidgetItem* ChatScreen::createChatListItem(const QString &chatName, const QString &chatID)
{
    QListWidgetItem *item = new QListWidgetItem(chatName, ui->chatsList);
    item->setData(Qt::UserRole, chatID);
    item->setIcon(QIcon(":/images/default_avatar.png")); // Установите иконку по умолчанию
    return item;
}

void ChatScreen::displayMessages(const QJsonArray &messages)
{
    ui->messagesList->clear(); // Очищаем список сообщений перед добавлением
    for (const QJsonValue &msgVal : messages) {
        QJsonObject msgObj = msgVal.toObject();
        QString sender = msgObj.value("sender").toString();
        QString text = msgObj.value("text").toString();
        QString formattedMessage = sender + ": " + text;
        ui->messagesList->addItem(new QListWidgetItem(formattedMessage));
    }
}
void ChatScreen::onReadyRead()
{
    QByteArray responseData = socket->readAll();
    qDebug() << "Полученные данные от сервера:" << responseData;

    buffer.append(responseData);

    // Разделяем буфер на отдельные сообщения по символу '\n'
    while (buffer.contains('\n')) {
        int index = buffer.indexOf('\n');
        QByteArray message = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        if (message.isEmpty()) {
            continue; // Пропускаем пустые строки
        }

        QJsonParseError parseError;
        QJsonDocument responseDoc = QJsonDocument::fromJson(message, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
            qDebug() << "Некорректный JSON:" << message;
            continue; // Продолжаем обработку следующих сообщений
        }

        QJsonObject response = responseDoc.object();
        QString status = response.value("status").toString();
        QString messageText = response.value("message").toString();
        QString sessionIDResp = response.value("sessionID").toString();
        QString login = response.value("login").toString();

        qDebug() << "Статус ответа:" << status << ", сообщение:" << messageText;

        if (response.contains("type")) {
            QString responseType = response.value("type").toString();
            qDebug() << "Тип ответа:" << responseType;

            if (responseType == "newMessages") {
                QJsonObject newMessages = response.value("newMessages").toObject();
                qDebug() << "Получены newMessages:" << newMessages;

                if (!newMessages.isEmpty()) {
                    for (auto it = newMessages.begin(); it != newMessages.end(); ++it) {
                        QString chatID = it.key();
                        QJsonArray messagesArray = it.value().toArray();

                        if (!localMessageCache.contains(chatID)) {
                            QJsonObject chatCache;
                            chatCache["messages"] = QJsonArray();
                            localMessageCache.insert(chatID, chatCache);
                            qDebug() << "Инициализирован локальный кэш для chatID:" << chatID;
                        }

                        QJsonArray existingMessages = localMessageCache[chatID].value("messages").toArray();

                        for (const QJsonValue &msgVal : messagesArray) {
                            QJsonObject msgObj = msgVal.toObject();
                            existingMessages.append(msgObj);
                            if (chatID == currentChatID) {
                                QString sender = msgObj.value("sender").toString();
                                QString text = msgObj.value("text").toString();
                                QString formattedMessage = sender + ": " + text;
                                ui->messagesList->addItem(new QListWidgetItem(formattedMessage));
                                qDebug() << "Добавлено сообщение в UI:" << formattedMessage;
                            }
                        }

                        localMessageCache[chatID]["messages"] = existingMessages;
                        qDebug() << "Обновлён локальный кэш для chatID:" << chatID;
                    }
                    saveLocalCache();
                    qDebug() << "Локальный кэш сохранён после обработки newMessages.";
                }
            }
            else if (responseType == "chatList") {
                // Обработка списка чатов
                QJsonArray chats = response.value("chats").toArray();
                ui->chatsList->clear();
                chatAvatars.clear();

                qDebug() << "Получен список чатов, количество:" << chats.size();

                for (const QJsonValue &chatVal : chats) {
                    QJsonObject chatObj = chatVal.toObject();
                    QString chatID = chatObj.value("id").toString();
                    QString chatName = chatObj.value("name").toString();
                    QListWidgetItem *item = createChatListItem(chatName, chatID);
                    ui->chatsList->addItem(item);

                    // Запрос аватара для этого чата, если он есть
                    if (!chatObj.value("avatarPath").toString().isEmpty()) {
                        fetchChatAvatar(chatID);
                    }

                    // Инициализация локального кэша для нового чата, если он еще не существует
                    if (!localMessageCache.contains(chatID)) {
                        QJsonObject newChatCache;
                        newChatCache["messages"] = QJsonArray();
                        localMessageCache.insert(chatID, newChatCache);
                        qDebug() << "Инициализирован локальный кэш для chatID:" << chatID;
                    }
                }

                // Сохраняем обновлённый локальный кэш
                saveLocalCache();
                qDebug() << "Локальный кэш сохранён после обработки chatList.";

                // Инициируем синхронизацию сообщений
                syncMessagesWithServer();
                qDebug() << "Инициирована синхронизация сообщений после получения списка чатов.";
            }
            else if (responseType == "createChat") {
                // Обработка ответа на создание чата
                if (response.contains("chatID")) {
                    QString chatID = response["chatID"].toString();
                    pendingChatID = chatID; // Сохраняем chatID
                    qDebug() << "Получен chatID нового чата:" << chatID;
                }

                if (!pendingAvatarData.isEmpty() && !pendingChatID.isEmpty()) {
                    sendUploadAvatarRequest(); // Отправляем JSON-запрос uploadAvatar
                }
                loadChats(); // Обновляем список чатов
                qDebug() << "Обновлён список чатов после создания нового чата.";
            }
            else if (responseType == "loadMessages") {
                // Обработка ответа на загрузку сообщений
                if (response.contains("messages")) {
                    QJsonArray messages = response.value("messages").toArray();
                    qDebug() << "Получены сообщения для чата:" << currentChatID << ", количество:" << messages.size();

                    // Очистка текущего списка сообщений
                    ui->messagesList->clear();

                    // Отображение полученных сообщений
                    displayMessages(messages);

                    // Обновление локального кэша
                    if (!localMessageCache.contains(currentChatID)) {
                        QJsonObject chatCache;
                        chatCache["messages"] = QJsonArray();
                        localMessageCache.insert(currentChatID, chatCache);
                    }

                    QJsonArray existingMessages = localMessageCache[currentChatID].value("messages").toArray();
                    for (const QJsonValue &msgVal : messages) {
                        QJsonObject msgObj = msgVal.toObject();
                        existingMessages.append(msgObj);
                    }
                    localMessageCache[currentChatID]["messages"] = existingMessages;

                    saveLocalCache();
                    qDebug() << "Сообщения загружены и сохранены в локальном кэше.";
                }
                else {
                    qDebug() << "Ответ на loadMessages не содержит поле 'messages'.";
                }
            }
            else {
                qDebug() << "Неизвестный тип ответа:" << responseType;
                // Добавьте обработку других типов сообщений при необходимости
            }
        }

        // Проверка наличия аватара в ответе
        if (response.contains("avatar")) {
            QString chatID = response.value("chatID").toString();
            QString avatarBase64 = response.value("avatar").toString();
            QByteArray avatarBytes = QByteArray::fromBase64(avatarBase64.toUtf8());
            QImage avatarImage;
            if (avatarImage.loadFromData(avatarBytes)) {
                displayChatAvatar(chatID, avatarImage);
                qDebug() << "Аватар для chatID:" << chatID << "успешно загружен и отображён.";
            }
            else {
                qDebug() << "Не удалось загрузить аватар для chatID:" << chatID;
            }
        }
        else if (messageText == "Avatar uploaded successfully.") {
            QMessageBox::information(this, "Успех", "Аватар чата успешно загружен.");
            loadChats(); // Обновляем список чатов для отображения нового аватара
        }
        else if (response.contains("username")) {
            // Обработка ответа на checkUser запрос
            QString username = response.value("username").toString();
            bool exists = (messageText == "User exists.");
            emit userCheckCompleted(username, exists);
        }
    }
}

void ChatScreen::loadLocalCache()
{
    QFile file("local_messages.json");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                for (auto it = obj.begin(); it != obj.end(); ++it) {
                    if (it.value().isObject()) {
                        localMessageCache.insert(it.key(), it.value().toObject());
                        qDebug() << "Загружено сообщение для chatID:" << it.key();
                    }
                }
                qDebug() << "Local message cache loaded successfully.";
            }
            else {
                qDebug() << "Failed to parse local message cache JSON.";
            }
            file.close();
        }
        else {
            qDebug() << "Failed to open local_messages.json for reading.";
        }
    }
    else {
        qDebug() << "Файл local_messages.json не найден. Начинаем с пустого кэша.";
    }
}

void ChatScreen::saveLocalCache()
{
    QFile file("local_messages.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        for (auto it = localMessageCache.begin(); it != localMessageCache.end(); ++it) {
            obj.insert(it.key(), it.value());
        }
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
        qDebug() << "Local message cache saved successfully.";
    }
    else {
        qDebug() << "Failed to save local message cache.";
    }
}

void ChatScreen::syncMessagesWithServer()
{
    QJsonObject request;
    request["type"] = "syncMessages";
    request["sessionID"] = sessionID;

    QJsonObject lastMessages;

    // Для каждого чата в локальном кэше определяем последний timestamp
    for (auto it = localMessageCache.begin(); it != localMessageCache.end(); ++it) {
        QString chatID = it.key();
        QJsonObject chatObj = it.value();

        if (chatObj.contains("messages")) {
            QJsonArray messages = chatObj["messages"].toArray();
            if (!messages.isEmpty()) {
                // Предполагаем, что сообщения отсортированы по времени
                QJsonObject lastMessage = messages.last().toObject();
                qint64 lastTimestamp = lastMessage.value("timestamp").toInt();
                lastMessages.insert(chatID, lastTimestamp);
            }
        }
    }

    request["lastMessages"] = lastMessages;
    qDebug() << "Last Messages for Sync:" << lastMessages;

    sendRequest(request);
    qDebug() << "Sent syncMessages request to server:" << QJsonDocument(request).toJson(QJsonDocument::Compact) + "\n";
}

void ChatScreen::sendUploadAvatarRequest()
{
    QJsonObject avatarRequest;
    avatarRequest["type"] = "uploadAvatar";
    avatarRequest["sessionID"] = sessionID;
    avatarRequest["chatID"] = pendingChatID;

    // Кодируем avatarData в Base64
    QString avatarBase64 = QString(pendingAvatarData.toBase64());

    avatarRequest["avatar"] = avatarBase64;

    sendRequest(avatarRequest);
    qDebug() << "Отправка запроса uploadAvatar с аватаром:" << QJsonDocument(avatarRequest).toJson(QJsonDocument::Compact) + "\n";

    // Очистка данных после отправки
    pendingAvatarData.clear();
    pendingChatID.clear();
}

