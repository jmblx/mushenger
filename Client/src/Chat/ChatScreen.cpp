#include "ChatScreen.h"
#include "ui_ChatScreen.h"
#include "src/ThemeManager/ThemeManager.h"
#include "src/Profile/ProfileScreen.h"
#include "messagewidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QBuffer>
#include <QImage>
#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QPainter>
#include <QThread>
#include "chatitemdelegate.h"
#include <QMenu>

ChatScreen::ChatScreen(const QString &sessionID, const QString &userLogin, QTcpSocket *existingSocket, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatScreen), socket(existingSocket),
    sessionID(sessionID), currentUserLogin(userLogin)
{
    ui->setupUi(this);
    ui->userName->setText(currentUserLogin);

    // Отключаем все существующие подключения readyRead
    QObject::disconnect(socket, nullptr, nullptr, nullptr);

    chatItemDelegate = new ChatItemDelegate(this);
    ui->chatsList->setItemDelegate(chatItemDelegate);

    // Подключение сигналов и слотов
    connect(ui->chatsList, &QListWidget::itemClicked, this, &ChatScreen::onChatSelected);
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatScreen::onSendMessageClicked);
    connect(socket, &QTcpSocket::readyRead, this, &ChatScreen::onReadyRead);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &ChatScreen::onEnterPressed);
    connect(ui->newChat, &QPushButton::clicked, this, &ChatScreen::onNewChatClicked);
    connect(this, &ChatScreen::userCheckCompleted, this, &ChatScreen::onUserCheckCompleted);
    connect(chatItemDelegate, &ChatItemDelegate::gearIconClicked, this, &ChatScreen::onGearIconClicked);
    connect(ui->avatarButton, &QPushButton::clicked, this, &ChatScreen::onAvatarClicked);
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &ChatScreen::onThemeSwitchButtonClicked);
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &ChatScreen::onThemeChanged);
    onThemeChanged(ThemeManager::instance().currentTheme());

    ui->userName->setText(currentUserLogin);
    ui->chatsList->setIconSize(QSize(50, 50)); // Увеличьте до 50x50 пикселей

    ui->chatsList->setViewMode(QListView::ListMode); // Убедитесь, что используется ListMode
    ui->chatsList->setSpacing(10);

    loadChats();

    ui->messagesList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Устанавливаем режим подгонки размера элементов
    ui->messagesList->setResizeMode(QListWidget::Adjust);

    // Устанавливаем флаг для поддержки вертикального списка
    ui->messagesList->setFlow(QListView::TopToBottom);

    // Включаем перенос слов в QListWidget
    ui->messagesList->setWordWrap(true);
    ui->messagesList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    loadLocalCache();
    getUserData();
    loadLocalCache();
    loadAvatarSyncData(); // Загрузка данных синхронизации аватарок
}

ChatScreen::~ChatScreen()
{
    delete ui;
}

void ChatScreen::getUserData()
{
    QJsonObject request;
    request["type"] = "getUserData";
    request["sessionID"] = sessionID;
    sendRequest(request);
}

void ChatScreen::displayUserAvatar(const QImage &avatar)
{
    currentUserAvatar = avatar;

    // Преобразуем изображение в нужный размер и делаем его круглым
    QImage scaledImage = avatar.scaled(90, 90, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pixmap = QPixmap::fromImage(scaledImage);

    QPixmap circularPixmap(90, 90);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 90, 90);

    // Устанавливаем аватарку в QLabel или QPushButton
    ui->userAvatar->setPixmap(circularPixmap);
}

void ChatScreen::onAvatarClicked()
{
    qDebug() << "Переход на экран профиля";

    ProfileScreen *profileScreen = new ProfileScreen(sessionID, currentUserLogin, socket);
    profileScreen->show();
    this->hide();
}

void ChatScreen::updateUserAvatar(const QImage &image)
{
    currentUserAvatar = image;

    QImage scaledImage = image.scaled(90, 90, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pixmap = QPixmap::fromImage(scaledImage);

    QPixmap circularPixmap(90, 90);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 90, 90);

    ui->userAvatar->setPixmap(circularPixmap);
}


void ChatScreen::onThemeSwitchButtonClicked()
{
    ThemeManager::instance().toggleTheme();
}

void ChatScreen::onThemeChanged(const QString& newTheme)
{
    QString backgroundPath = QString(":/images/%1/chat_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);
    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Failed to load background image:" << backgroundPath;
    }

    QIcon sendIcon(QString(":/images/%1/send_message_icon.svg").arg(newTheme));
    ui->sendButton->setIcon(sendIcon);

    if (!currentUserAvatar.isNull()) {
        displayUserAvatar(currentUserAvatar);
    } else {
        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
        QPixmap defaultAvatar(defaultAvatarPath);
        if (!defaultAvatar.isNull()) {
            ui->userAvatar->setPixmap(defaultAvatar);
        } else {
            qDebug() << "Failed to load default avatar image:" << defaultAvatarPath;
        }
    }

    for(int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        QString chatID = item->data(Qt::UserRole).toString();
        if(chatAvatars.contains(chatID)) {
            item->setIcon(QIcon(QPixmap::fromImage(chatAvatars[chatID])));
        }
        else {
            QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
            item->setIcon(QIcon(defaultAvatarPath));
        }
    }
}

void ChatScreen::sendRequest(const QJsonObject &request)
{
    QJsonDocument doc(request);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n";
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

    qDebug() << "Selected chat with ID:" << chatID;

    // Загрузка сообщений из локального кэша
    if (localMessageCache.contains(chatID)) {
        QJsonArray localMessages = localMessageCache[chatID].value("messages").toArray();
        ui->messagesList->clear();
        displayMessages(localMessages);
    }

    // Получение списка участников чата
    QStringList participants = chatParticipants.value(chatID);

    // Подготовка списка пользователей и их последних обновлений аватарок, исключая текущего пользователя
    QJsonObject usersToSync;
    for (const QString &user : participants) {
        if (user == currentUserLogin) continue; // Исключаем себя
        qint64 lastSync = 0;
        if (avatarSyncData.contains(user)) {
            lastSync = avatarSyncData[user].toInt();
        }
        usersToSync.insert(user, lastSync);
    }

    // Отправка запроса на сервер
    QJsonObject request;
    request["type"] = "getUserAvatars";
    request["users"] = usersToSync;
    sendRequest(request);
    qDebug() << "Sent getUserAvatars request for users:" << usersToSync.keys();

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

    // Get current UTC timestamp
    qint64 currentTimestamp = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();

    // Get avatar pixmap for current user
    QPixmap avatarPixmap;
    if (!currentUserAvatar.isNull()) {
        avatarPixmap = QPixmap::fromImage(currentUserAvatar);
    } else {
        // Use default avatar
        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
        avatarPixmap = QPixmap(defaultAvatarPath);
    }

    bool isDefaultAvatar = false;
    if (currentUserAvatar.isNull()) {
        isDefaultAvatar = true;
    }

    // Create MessageWidget for the sent message with timestamp
    MessageWidget *msgWidget = new MessageWidget(currentUserLogin, messageText, avatarPixmap, false, true, currentTimestamp, isDefaultAvatar, this);

    QListWidgetItem *item = new QListWidgetItem(ui->messagesList);
    item->setSizeHint(msgWidget->sizeHint());
    ui->messagesList->addItem(item);
    ui->messagesList->setItemWidget(item, msgWidget);

    // Scroll down
    ui->messagesList->scrollToBottom();
    qDebug() << "Added sender's message to UI as MessageWidget.";

    // Add message to local cache
    if (!localMessageCache.contains(currentChatID)) {
        QJsonObject chatCache;
        chatCache["messages"] = QJsonArray();
        localMessageCache.insert(currentChatID, chatCache);
    }

    QJsonArray existingMessages = localMessageCache[currentChatID].value("messages").toArray();
    QJsonObject newMessage;
    newMessage["sender"] = currentUserLogin;
    newMessage["text"] = messageText;
    newMessage["timestamp"] = currentTimestamp;
    existingMessages.append(newMessage);
    localMessageCache[currentChatID]["messages"] = existingMessages;

    saveLocalCache();
    qDebug() << "Message added to local cache and saved.";
}

void ChatScreen::addMessageToCache(const QString &sender, const QString &text)
{
    if (!localMessageCache.contains(currentChatID)) {
        QJsonObject chatCache;
        chatCache["messages"] = QJsonArray();
        localMessageCache.insert(currentChatID, chatCache);
    }

    QJsonArray existingMessages = localMessageCache[currentChatID].value("messages").toArray();
    QJsonObject newMessage;
    newMessage["sender"] = sender;
    newMessage["text"] = text;
    newMessage["timestamp"] = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    existingMessages.append(newMessage);
    localMessageCache[currentChatID]["messages"] = existingMessages;

    saveLocalCache();
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
    qDebug() << "biig bob" << avatarPath;
    if (!avatarPath.isEmpty()) {
        pendingAvatarData = processAvatar(avatarPath);
        avatarSelected = !pendingAvatarData.isEmpty();
        qDebug() << "mr bobby fisher" << avatarSelected;
        if (pendingAvatarData.isEmpty()) {
            avatarSelected = false;
            return;  // Если обработка аватарки не удалась
        }
    }
    else {
        avatarSelected = false;  // Аватарка не выбрана
        pendingAvatarData.clear();
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
    QJsonObject createChatRequest;
    createChatRequest["type"] = "createChat";
    createChatRequest["sessionID"] = sessionID;
    createChatRequest["chatName"] = chatName;

    QJsonArray participantsArray;
    for (const QString &participant : validParticipants) {
        participantsArray.append(participant);
    }
    createChatRequest["participants"] = participantsArray;

    if (avatarSelected) {
        qDebug() << "eblo" << avatarSelected;
        createChatRequest["avatar"] = "true";  // Булевое значение для флага
    }

    sendRequest(createChatRequest);
    qDebug() << "Отправка запроса на создание чата:" << QJsonDocument(createChatRequest).toJson(QJsonDocument::Compact) + "\n";
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
    if (avatar.isNull()) {
        qDebug() << "Пустая аватарка для chatID:" << chatID;
        return;
    }

    QImage scaledImage = avatar.scaled(90, 90, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap pixmap = QPixmap::fromImage(scaledImage);

    QPixmap circularPixmap(90, 90);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);

    painter.drawEllipse(0, 0, 90, 90);

    // Найдите элемент списка чатов по chatID и установите иконку
    for(int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        if(item->data(Qt::UserRole).toString() == chatID) {
            QIcon icon(circularPixmap);  // Используем круговой аватар
            item->setIcon(icon);
            chatAvatars[chatID] = avatar; // Сохраняем аватар
            qDebug() << "Установлен круговой аватар для chatID:" << chatID;
            break;
        }
    }

    // Увеличиваем размер элемента, если это необходимо
    for(int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        item->setSizeHint(QSize(60, 60)); // Увеличьте высоту до 60 пикселей
    }
}

QListWidgetItem* ChatScreen::createChatListItem(const QString &chatName, const QString &chatID)
{
    QListWidgetItem *item = new QListWidgetItem(chatName, ui->chatsList);
    item->setData(Qt::UserRole, chatID);

    // Используем путь, зависящий от текущей темы
    QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
    item->setIcon(QIcon(defaultAvatarPath));

    return item;
}

void ChatScreen::displayMessages(const QJsonArray &messages)
{
    ui->messagesList->clear(); // Очистить список сообщений перед добавлением

    for (int i = 0; i < messages.size(); ++i) {
        QJsonObject msgObj = messages[i].toObject();
        QString sender = msgObj.value("sender").toString();
        QString text = msgObj.value("text").toString();
        qint64 timestamp = msgObj.value("timestamp").toInt();

        bool isTransparent = false;

        // Проверяем, является ли следующий отправитель тем же
        if (i < messages.size() - 1) {
            QJsonObject nextMsg = messages[i + 1].toObject();
            QString nextSender = nextMsg.value("sender").toString();
            if (nextSender == sender) {
                isTransparent = true;
            }
        }

        bool isCurrentUser = (sender == currentUserLogin);

        // Получаем аватарку для отправителя
        QPixmap avatarPixmap;
        bool isDefaultAvatar = false;

        if (isCurrentUser) {
            // Используем только currentUserAvatar для себя
            if (!currentUserAvatar.isNull()) {
                avatarPixmap = QPixmap::fromImage(currentUserAvatar);
            } else {
                // Используем дефолтную аватарку
                QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
                avatarPixmap = QPixmap(defaultAvatarPath);
                isDefaultAvatar = true;
            }
        } else {
            // Для других пользователей используем userAvatars
            if (userAvatars.contains(sender)) {
                avatarPixmap = userAvatars[sender];
            } else {
                // Используем дефолтную аватарку
                QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
                avatarPixmap = QPixmap(defaultAvatarPath);
                isDefaultAvatar = true;
            }
        }

        // Создаём MessageWidget с аватаркой и информацией об отправителе
        MessageWidget *msgWidget = new MessageWidget(sender, text, avatarPixmap, isTransparent, isCurrentUser, timestamp, isDefaultAvatar, this);

        // Создаём элемент списка и устанавливаем размер
        QListWidgetItem *item = new QListWidgetItem(ui->messagesList);
        item->setSizeHint(msgWidget->sizeHint());

        // Добавляем элемент в список и устанавливаем виджет
        ui->messagesList->addItem(item);
        ui->messagesList->setItemWidget(item, msgWidget);
    }

    ui->messagesList->scrollToBottom(); // Прокрутка вниз после добавления сообщений
}

void ChatScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Обновляем максимальную ширину для всех сообщений
    for(int i = 0; i < ui->messagesList->count(); ++i){
        QListWidgetItem *item = ui->messagesList->item(i);
        QWidget *widget = ui->messagesList->itemWidget(item);
        MessageWidget *msgWidget = qobject_cast<MessageWidget*>(widget);
        if(msgWidget){
            QLabel *msgLabel = msgWidget->findChild<QLabel*>("messageLabel");
            if(msgLabel){
                msgLabel->setMaximumWidth(this->width() * 0.7);
            }
        }
    }
}

void ChatScreen::onReadyRead()
{
    QByteArray responseData = socket->readAll();
    qDebug() << "Полученные данные от сервера:" << responseData;

    buffer.append(responseData);

    while (buffer.contains('\n')) {
        int index = buffer.indexOf('\n');
        QByteArray message = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        if (message.isEmpty()) {
            continue;
        }

        QJsonParseError parseError;
        QJsonDocument responseDoc = QJsonDocument::fromJson(message, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
            qDebug() << "Некорректный JSON:" << message;
            continue;
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

            if (responseType == "avatars") {
                QJsonObject avatars = response.value("avatars").toObject();
                qDebug() << "Получены аватарки:" << avatars;

                for (auto it = avatars.begin(); it != avatars.end(); ++it) {
                    QString username = it.key();
                    QJsonObject avatarInfo = it.value().toObject();
                    qint64 serverLastUpdate = avatarInfo.value("avatarLastUpdated").toInt();
                    QString avatarBase64 = avatarInfo.value("avatar").toString();

                    QByteArray avatarBytes = QByteArray::fromBase64(avatarBase64.toUtf8());
                    QImage avatarImage;
                    if (avatarImage.loadFromData(avatarBytes)) {
                        QPixmap avatarPixmap = QPixmap::fromImage(avatarImage);
                        userAvatars.insert(username, avatarPixmap);
                        avatarSyncData.insert(username, serverLastUpdate);
                        qDebug() << "Аватарка обновлена для пользователя:" << username;
                    } else {
                        qDebug() << "Не удалось декодировать аватарку для пользователя:" << username;
                    }
                }
                saveAvatarSyncData();

                if (!currentChatID.isEmpty()) {
                    QJsonArray messages = localMessageCache[currentChatID].value("messages").toArray();
                    ui->messagesList->clear();
                    displayMessages(messages);
                }
                qDebug() << "Локальный кэш сохранён после обработки avatars.";
            }
            else if (responseType == "newMessages") {
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
                                qint64 timestamp = msgObj.value("timestamp").toInt();

                                bool isCurrentUser = (sender == currentUserLogin);
                                bool isTransparent = false;

                                if (ui->messagesList->count() > 0) {
                                    QListWidgetItem *lastItem = ui->messagesList->item(ui->messagesList->count() - 1);
                                    MessageWidget *lastWidget = qobject_cast<MessageWidget*>(ui->messagesList->itemWidget(lastItem));
                                    if (lastWidget && lastWidget->getSender() == sender) {
                                        isTransparent = true;
                                    }
                                }

                                QPixmap avatarPixmap;
                                if (isCurrentUser) {
                                    if (!currentUserAvatar.isNull()) {
                                        avatarPixmap = QPixmap::fromImage(currentUserAvatar);
                                    } else {
                                        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
                                        avatarPixmap = QPixmap(defaultAvatarPath);
                                    }
                                } else {
                                    if (userAvatars.contains(sender)) {
                                        avatarPixmap = userAvatars[sender];
                                    } else {
                                        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
                                        avatarPixmap = QPixmap(defaultAvatarPath);
                                    }
                                }

                                bool isDefaultAvatar = false;
                                if (!isCurrentUser && !userAvatars.contains(sender)) {
                                    isDefaultAvatar = true;
                                }

                                MessageWidget *msgWidget = new MessageWidget(sender, text, avatarPixmap, isTransparent, isCurrentUser, timestamp, isDefaultAvatar, this);

                                QListWidgetItem *item = new QListWidgetItem(ui->messagesList);
                                item->setSizeHint(msgWidget->sizeHint());
                                ui->messagesList->addItem(item);
                                ui->messagesList->setItemWidget(item, msgWidget);

                                ui->messagesList->scrollToBottom();
                                qDebug() << "Добавлено новое сообщение в UI как MessageWidget.";
                            }
                        }

                        localMessageCache[chatID]["messages"] = existingMessages;
                        qDebug() << "Обновлён локальный кэш для chatID:" << chatID;
                    }
                    saveAvatarSyncData();

                    if (!currentChatID.isEmpty()) {
                        QJsonArray messages = localMessageCache[currentChatID].value("messages").toArray();
                        ui->messagesList->clear();
                        displayMessages(messages);
                    }
                    qDebug() << "Локальный кэш сохранён после обработки newMessages.";
                }
            }
            else if (responseType == "chatList") {
                QJsonArray chats = response.value("chats").toArray();
                ui->chatsList->clear();
                chatAvatars.clear();
                chatParticipants.clear();
                chatAdmins.clear();

                qDebug() << "Получен список чатов, количество:" << chats.size();

                for (const QJsonValue &chatVal : chats) {
                    QJsonObject chatObj = chatVal.toObject();
                    QString chatID = chatObj.value("id").toString();
                    QString chatName = chatObj.value("name").toString();
                    QString adminLogin = chatObj.value("admin").toString();

                    qDebug() << "Обрабатывается чат:" << chatName << "ID:" << chatID << "Admin:" << adminLogin;

                    QListWidgetItem *item = createChatListItem(chatName, chatID);
                    ui->chatsList->addItem(item);

                    if (adminLogin == currentUserLogin) {
                        chatAdmins.insert(chatID, adminLogin);
                        qDebug() << "Текущий пользователь является администратором чата:" << chatID;
                    }

                    QJsonArray participantsArray = chatObj.value("participants").toArray();
                    QStringList participantsList;

                    qDebug() << "Участники для чата" << chatID << ":";
                    for (const QJsonValue &participantVal : participantsArray) {
                        qDebug() << participantVal.toString();
                        participantsList << participantVal.toString();
                    }

                    if (!participantsList.contains(currentUserLogin)) {
                        participantsList.append(currentUserLogin);
                    }

                    chatParticipants.insert(chatID, participantsList);

                    // **Добавление новых участников в avatarSyncData с avatarLastUpdate = 0**
                    for (const QString &participant : participantsList) {
                        if (participant != currentUserLogin && !avatarSyncData.contains(participant)) {
                            avatarSyncData.insert(participant, 0); // Устанавливаем 0 для новых участников
                        }
                    }

                    saveAvatarSyncData(); // Сохраняем обновления

                    if (!chatObj.value("avatarPath").toString().isEmpty()) {
                        fetchChatAvatar(chatID);
                    }

                    if (!localMessageCache.contains(chatID)) {
                        QJsonObject newChatCache;
                        newChatCache["messages"] = QJsonArray();
                        localMessageCache.insert(chatID, newChatCache);
                        qDebug() << "Инициализирован локальный кэш для chatID:" << chatID;
                    }
                }

                chatItemDelegate->setAdminChats(chatAdmins);
                qDebug() << "Передача списка администраторов в делегат";

                updateChatCount();

                saveLocalCache();
                qDebug() << "Локальный кэш сохранён после обработки chatList.";

                syncMessagesWithServer();
                qDebug() << "Инициирована синхронизация сообщений после получения списка чатов.";
            }
            else if (responseType == "chatDeleted") {
                QString chatID = response.value("chatID").toString();
                onChatDeleted(chatID);
            }
            else if (responseType == "chatRenamed") {
                QString chatID = response.value("chatID").toString();
                QString newName = response.value("newName").toString();
                onChatRenamed(chatID, newName);
            }
            else if (responseType == "createChat") {
                if (response.contains("chatID")) {
                    QString chatID = response["chatID"].toString();
                    pendingChatID = chatID;
                    qDebug() << "Получен chatID нового чата:" << chatID;
                }

                if (avatarSelected && !pendingChatID.isEmpty()) {
                    qDebug() << "Отправляем запрос uploadAvatar для chatID:" << pendingChatID;
                    sendUploadAvatarRequest();
                }
                loadChats();
                updateChatCount();
            }
            else if (responseType == "newChat") {
                qDebug() << "Получено сообщение о новом чате, обновляем список чатов.";
                loadChats();
            }
            else if (responseType == "loadMessages") {
                if (response.contains("messages")) {
                    QJsonArray messages = response.value("messages").toArray();
                    qDebug() << "Получены сообщения для чата:" << currentChatID << ", количество:" << messages.size();

                    ui->messagesList->clear();

                    displayMessages(messages);

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
            else if (responseType == "userData") {
                QJsonObject userObj = response["user"].toObject();
                QString avatarBase64 = userObj.value("avatar").toString();

                if (!avatarBase64.isEmpty()) {
                    QByteArray avatarData = QByteArray::fromBase64(avatarBase64.toUtf8());
                    QImage avatarImage;
                    avatarImage.loadFromData(avatarData);
                    displayUserAvatar(avatarImage);
                    qDebug() << "Аватар пользователя успешно загружен и отображен.";
                }
                else {
                    qDebug() << "Аватар пользователя отсутствует.";
                }
            }
            else if (responseType == "checkUser") {
                QString username = response["username"].toString();
                bool exists = (status == "success");
                emit userCheckCompleted(username, exists);
            }
            else {
                qDebug() << "Неизвестный тип ответа:" << responseType;
            }
        }
        else if (messageText == "Chat created successfully.") {
            if (response.contains("chatID")) {
                QString chatID = response["chatID"].toString();
                pendingChatID = chatID;
            }

            if (!pendingAvatarData.isEmpty() && !pendingChatID.isEmpty()) {
                sendUploadAvatarRequest();
            }
            loadChats();
        }
        else if (messageText == "Ready to receive avatar data.") {
            if (!pendingAvatarData.isEmpty() && !pendingChatID.isEmpty()) {
                socket->write(pendingAvatarData);
                socket->flush();

                if (!socket->waitForBytesWritten(3000)) {
                    qDebug() << "Ошибка при отправке данных аватара";
                    QMessageBox::warning(this, "Ошибка", "Не удалось отправить аватар.");
                } else {
                    qDebug() << "Аватар успешно отправлен!";
                    QMessageBox::information(this, "Успех", "Аватар успешно загружен.");
                }

                pendingAvatarData.clear();
                pendingChatID.clear();
            }
        }
        else if (messageText == "Avatar uploaded successfully.") {
            QMessageBox::information(this, "Успех", "Аватар чата успешно загружен.");
            loadChats();
        }
        else if (messageText == "Avatar data.") {
            QString chatID = response["chatID"].toString();
            QString avatarBase64 = response["avatar"].toString();
            QByteArray avatarBytes = QByteArray::fromBase64(avatarBase64.toUtf8());
            QImage avatarImage;
            avatarImage.loadFromData(avatarBytes);
            displayChatAvatar(chatID, avatarImage);
        }
        else if (response.contains("username")) {
            QString username = response["username"].toString();
            bool exists = (messageText == "User exists.");
            emit userCheckCompleted(username, exists);
        }
        else if (response.contains("avatars")) {
            QJsonObject avatars = response.value("avatars").toObject();
            for (auto it = avatars.begin(); it != avatars.end(); ++it) {
                QString username = it.key();
                QJsonObject avatarInfo = it.value().toObject();
                qint64 serverLastUpdate = avatarInfo.value("avatarLastUpdated").toInt();
                QString avatarBase64 = avatarInfo.value("avatar").toString();

                QByteArray avatarBytes = QByteArray::fromBase64(avatarBase64.toUtf8());
                QImage avatarImage;
                avatarImage.loadFromData(avatarBytes);
                QPixmap avatarPixmap = QPixmap::fromImage(avatarImage);

                userAvatars.insert(username, avatarPixmap);
                qDebug() << "Loaded avatar into memory for user:" << username;

                avatarSyncData.insert(username, serverLastUpdate);
            }

            saveAvatarSyncData();

            if (!currentChatID.isEmpty()) {
                loadMessages(currentChatID);
            }
        }
        else {
            qDebug() << "Неизвестное сообщение от сервера: " << messageText;
        }
    }
}

void ChatScreen::loadAvatarSyncData()
{
    QFile file("avatar_sync.json");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                avatarSyncData = doc.object();
                qDebug() << "Avatar synchronization data loaded.";
            }
            else {
                qDebug() << "Failed to parse avatar_sync.json.";
            }
            file.close();
        }
        else {
            qDebug() << "Failed to open avatar_sync.json for reading.";
        }
    }
    else {
        qDebug() << "avatar_sync.json not found. Starting with empty synchronization data.";
        avatarSyncData = QJsonObject();
    }
}

void ChatScreen::saveAvatarSyncData()
{
    QFile file("avatar_sync.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(avatarSyncData);
        file.write(doc.toJson());
        file.close();
        qDebug() << "Avatar synchronization data saved.";
    }
    else {
        qDebug() << "Failed to save avatar_sync.json.";
    }
}

void ChatScreen::onGearIconClicked(const QModelIndex &index)
{
    QString chatID = index.data(Qt::UserRole).toString();
    QString chatName = index.data(Qt::DisplayRole).toString();
    qDebug() << "Шестерёнка нажата для чата:" << chatName;

    QMenu menu;
    QAction *renameAction = menu.addAction("Переименовать чат");
    QAction *deleteAction = menu.addAction("Удалить чат");
    QAction *selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == renameAction) {
        bool ok;
        QString newChatName = QInputDialog::getText(this, "Переименовать чат", "Введите новое название чата:", QLineEdit::Normal, chatName, &ok);
        if (ok && !newChatName.isEmpty()) {
            qDebug() << "Переименовываем чат в:" << newChatName;
            renameChat(chatID, newChatName);  // Отправляем запрос на сервер для переименования
        }
    } else if (selectedAction == deleteAction) {
        int result = QMessageBox::warning(this, "Удалить чат", "Вы действительно хотите удалить чат?", QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            qDebug() << "Удаляем чат:" << chatName;
            deleteChat(chatID);  // Отправляем запрос на сервер для удаления чата
        }
    }
}


void ChatScreen::deleteChat(const QString &chatID)
{
    QJsonObject request;
    request["type"] = "deleteChat";
    request["sessionID"] = sessionID;
    request["chatID"] = chatID;
    sendRequest(request);  // Отправляем запрос на сервер
}

void ChatScreen::renameChat(const QString &chatID, const QString &newName)
{
    QJsonObject request;
    request["type"] = "renameChat";
    request["sessionID"] = sessionID;
    request["chatID"] = chatID;
    request["newName"] = newName;
    sendRequest(request);  // Отправляем запрос на сервер
}

void ChatScreen::onChatDeleted(const QString &chatID)
{
    // Удаляем чат из интерфейса
    for (int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        if (item->data(Qt::UserRole).toString() == chatID) {
            delete ui->chatsList->takeItem(i);  // Удаляем элемент из списка
            break;
        }
    }

    // Удаляем чат из локального кэша
    localMessageCache.remove(chatID);
    saveLocalCache();
    qDebug() << "Чат с ID" << chatID << "был удалён.";

    // Обновляем счётчик чатов
    updateChatCount();
}


void ChatScreen::onChatRenamed(const QString &chatID, const QString &newName)
{
    // Обновляем название чата в интерфейсе
    for (int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        if (item->data(Qt::UserRole).toString() == chatID) {
            item->setText(newName);  // Обновляем название чата
            break;
        }
    }

    // Обновляем название чата в локальном кэше (при необходимости)
    if (localMessageCache.contains(chatID)) {
        QJsonObject chatCache = localMessageCache[chatID];
        chatCache["name"] = newName;
        localMessageCache[chatID] = chatCache;
    }
    saveLocalCache();
    qDebug() << "Чат с ID" << chatID << "был переименован в" << newName;
}

void ChatScreen::updateChatListIcons()
{
    for (int i = 0; i < ui->chatsList->count(); ++i) {
        QListWidgetItem *item = ui->chatsList->item(i);
        QString chatID = item->data(Qt::UserRole).toString();

        if (chatAdmins.contains(chatID) && chatAdmins[chatID] == currentUserLogin) {
            // Администратор — добавляем иконку шестеренки
            item->setIcon(QIcon(":/icons/gear_icon.png"));
        } else {
            // Убираем шестеренку для остальных пользователей
            item->setIcon(QIcon(":/images/default_avatar.png")); // Возвращаем иконку по умолчанию
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

    // **Отправка запроса на получение аватарок для участников**
    QJsonObject avatarRequest;
    avatarRequest["type"] = "getUserAvatars";
    QJsonObject usersToFetch;

    for (auto it = avatarSyncData.begin(); it != avatarSyncData.end(); ++it) {
        QString username = it.key();
        qint64 lastUpdate = it.value().toInt();
        usersToFetch.insert(username, lastUpdate);
    }

    avatarRequest["users"] = usersToFetch;

    sendRequest(avatarRequest);
    qDebug() << "Sent getUserAvatars request to server:" << QJsonDocument(avatarRequest).toJson(QJsonDocument::Compact) + "\n";

    // **Отправка запроса на синхронизацию сообщений**
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

void ChatScreen::updateChatCount()
{
    int count = ui->chatsList->count(); // Получаем количество чатов
    ui->chatCount->setText(QString::number(count)); // Устанавливаем текст счетчика
}
