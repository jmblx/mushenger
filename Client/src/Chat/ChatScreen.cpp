#include "ChatScreen.h"
#include "ui_ChatScreen.h"
#include "src/ThemeManager/ThemeManager.h"
#include "src/Profile/ProfileScreen.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QBuffer>
#include <QImage>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatScreen::ChatScreen(const QString &sessionID, const QString &userLogin, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ChatScreen),
    socket(new QTcpSocket(this)),
    sessionID(sessionID),
    currentUserLogin(userLogin)
{
    ui->setupUi(this);
    ui->userName->setText(currentUserLogin);

    connect(ui->chatsList, &QListWidget::itemClicked, this, &ChatScreen::onChatSelected);
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatScreen::onSendMessageClicked);
    connect(socket, &QTcpSocket::readyRead, this, &ChatScreen::onReadyRead);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &ChatScreen::onEnterPressed);
    connect(ui->newChat, &QPushButton::clicked, this, &ChatScreen::onNewChatClicked);
    connect(ui->avatarButton, &QPushButton::clicked, this, &ChatScreen::onAvatarClicked);
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &ChatScreen::onThemeSwitchButtonClicked);

    // Подключение к ThemeManager
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &ChatScreen::onThemeChanged);
    // Применение текущей темы
    onThemeChanged(ThemeManager::instance().currentTheme());

    connectToServer();
    loadChats();
}

ChatScreen::~ChatScreen()
{
    delete ui;
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

    QString profileIconPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
    QPixmap profileIcon(profileIconPath);
    if (!profileIcon.isNull()) {
        ui->userAvatar->setPixmap(profileIcon);
    } else {
        qDebug() << "Failed to load profile icon:" << profileIconPath;
    }
}

void ChatScreen::connectToServer()
{
    socket->connectToHost("127.0.0.1", 12345);
    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
        qApp->exit(); // Exit application if connection fails
    }
}

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
    request["type"] = "loadMessages";
    request["sessionID"] = sessionID;
    request["chatID"] = chatID;
    sendRequest(request);
}

void ChatScreen::onChatSelected(QListWidgetItem *item)
{
    QString chatID = item->data(Qt::UserRole).toString();
    loadMessages(chatID);
}

void ChatScreen::onSendMessageClicked()
{
    QString messageText = ui->messageInput->text();
    if (messageText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сообщение не может быть пустым.");
        return;
    }

    QJsonObject request;
    request["type"] = "message";
    request["sessionID"] = sessionID;
    request["chatID"] = currentChatID;
    request["message"] = messageText;
    sendRequest(request);
    ui->messageInput->clear();
}

void ChatScreen::sendRequest(const QJsonObject &request)
{
    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n"); // Add newline for separation
    socket->flush();
}

void ChatScreen::onReadyRead()
{
    while (socket->canReadLine()) {
        QByteArray responseData = socket->readLine();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (!responseDoc.isObject()) {
            QMessageBox::critical(this, "Ошибка", "Неверный формат ответа от сервера.");
            return;
        }

        QJsonObject response = responseDoc.object();
        QString status = response.value("status").toString();
        if (status == "success") {
            if (response.contains("chats")) {
                QJsonArray chats = response.value("chats").toArray();
                ui->chatsList->clear();
                for (const QJsonValue &chat : chats) {
                    QJsonObject chatObj = chat.toObject();
                    QListWidgetItem *item = new QListWidgetItem(chatObj.value("name").toString(), ui->chatsList);
                    item->setData(Qt::UserRole, chatObj.value("id").toString());
                    ui->chatsList->addItem(item);
                }
            } else if (response.contains("messages")) {
                QJsonArray messages = response.value("messages").toArray();
                ui->messagesList->clear();
                for (const QJsonValue &msg : messages) {
                    QJsonObject msgObj = msg.toObject();
                    QString messageText = msgObj.value("sender").toString() + ": " + msgObj.value("text").toString();
                    ui->messagesList->addItem(new QListWidgetItem(messageText));
                }
            }
        } else {
            QMessageBox::warning(this, "Ошибка", response.value("message").toString());
        }
    }
}

void ChatScreen::onEnterPressed()
{
    if (!ui->messageInput->text().isEmpty()) {
        onSendMessageClicked();
    }
}

void ChatScreen::onNewChatClicked()
{
    QString chatName = QInputDialog::getText(this, "Создание нового чата", "Введите название чата:");
    if (chatName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название чата не может быть пустым.");
        return;
    }

    QString avatarPath = QFileDialog::getOpenFileName(this, "Выберите аватарку", "", "Images (*.png *.jpg *.bmp)");
    QByteArray avatarData;
    if (!avatarPath.isEmpty()) {
        avatarData = processAvatar(avatarPath);
        if (avatarData.isEmpty()) {
            return;
        }
    }

    QStringList participants;
    participants.append(currentUserLogin);

    while (true) {
        QString newParticipant = QInputDialog::getText(this, "Добавление участника", "Введите логин участника:");
        if (newParticipant.isEmpty()) break;

        if (!checkUserExists(newParticipant)) {
            QMessageBox::warning(this, "Ошибка", "Пользователь не найден.");
            continue;
        }

        participants.append(newParticipant);
        QMessageBox::information(this, "Успех", QString("Пользователь %1 добавлен.").arg(newParticipant));
    }

    QJsonObject request;
    request["type"] = "createChat";
    request["sessionID"] = sessionID;
    request["chatName"] = chatName;
    request["hasAvatar"] = !avatarData.isEmpty();
    request["participants"] = QJsonArray::fromStringList(participants);

    sendRequest(request);

    if (!avatarData.isEmpty()) {
        sendAvatarToServer(avatarData, chatName);
    }
}

bool ChatScreen::checkUserExists(const QString &username)
{
    QJsonObject request;
    request["type"] = "checkUser";
    request["sessionID"] = sessionID;
    request["username"] = username;

    sendRequest(request);

    if (socket->waitForReadyRead(3000)) {
        QByteArray responseData = socket->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

        if (!responseDoc.isObject()) {
            QMessageBox::critical(this, "Ошибка", "Неверный формат ответа от сервера.");
            return false;
        }

        QJsonObject response = responseDoc.object();
        QString status = response.value("status").toString();
        return status == "success";
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ответ от сервера.");
        return false;
    }
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

void ChatScreen::sendAvatarToServer(const QByteArray &avatarData, const QString &chatName)
{
    QJsonObject avatarRequest;
    avatarRequest["type"] = "uploadAvatar";
    avatarRequest["sessionID"] = sessionID;
    avatarRequest["chatName"] = chatName;

    QJsonDocument doc(avatarRequest);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
    socket->write(avatarData);
    socket->flush();
}

void ChatScreen::onAvatarClicked()
{
    qDebug() << "Переход на экран профиля";

    ProfileScreen *profileScreen = new ProfileScreen(sessionID, currentUserLogin);
    profileScreen->show();
    this->hide();
}
