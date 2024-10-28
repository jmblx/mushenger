#include "src/ThemeManager/ThemeManager.h"
#include "src/Profile/ProfileScreen.h"
#include "LoginScreen.h"
#include "ui_LoginScreen.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "src/Chat/ChatScreen.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    ui->PwdInput->setEchoMode(QLineEdit::Password);

    connect(ui->LoginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &LoginScreen::onReadyRead);
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &LoginScreen::onThemeSwitchButtonClicked);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &LoginScreen::onThemeChanged);
    onThemeChanged(ThemeManager::instance().currentTheme());

    qDebug() << "Загрузка сессии при запуске...";
    loadSession();
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

void LoginScreen::onThemeSwitchButtonClicked()
{
    ThemeManager::instance().toggleTheme();
}

void LoginScreen::onThemeChanged(const QString& newTheme)
{
    QString backgroundPath = QString(":/images/%1/auth_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);

    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Не удалось загрузить фоновое изображение:" << backgroundPath;
    }
}

void LoginScreen::onLoginButtonClicked()
{
    QString login = ui->LoginInput->text();
    QString password = ui->PwdInput->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    qDebug() << "Нажата кнопка входа с логином:" << login;
    sendLoginRequest(login, password);
}

void LoginScreen::sendLoginRequest(const QString &login, const QString &password)
{
    qDebug() << "Отправка запроса на вход на сервер...";
    connectToServer();

    if (!socket->isOpen()) {
        qDebug() << "Сокет не открыт, не удалось подключиться к серверу.";
        return;
    }

    QJsonObject request;
    request["type"] = "login";
    request["login"] = login;
    request["password"] = password;

    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
    socket->flush();

    qDebug() << "Запрос на вход отправлен:" << request;
}

void LoginScreen::loadSession()
{
    QFile file("session.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString sessionID = in.readLine();
        if (!sessionID.isEmpty()) {
            qDebug() << "Загружена сессия из файла:" << sessionID;
            connectToServer();
            getUserData(sessionID);
        } else {
            qDebug() << "Файл сессии пуст.";
            showLoginScreen();
        }
    } else {
        qDebug() << "Ошибка открытия файла сессии для чтения:" << file.errorString();
        showLoginScreen();
    }
}

void LoginScreen::getUserData(const QString &sessionID)
{
    if (!socket->isOpen()) {
        qDebug() << "Сокет не открыт, не удается отправить запрос данных пользователя.";
        return;
    }

    QJsonObject request;
    request["type"] = "getUserData";
    request["sessionID"] = sessionID;

    qDebug() << "Запрос данных пользователя с сессией:" << sessionID;
    socket->write(QJsonDocument(request).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
}

void LoginScreen::connectToServer()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        qDebug() << "Попытка подключения к серверу...";
        socket->connectToHost("127.0.0.1", 12345);
        if (!socket->waitForConnected(3000)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
            qDebug() << "Не удалось подключиться к серверу.";
        } else {
            qDebug() << "Подключение к серверу успешно.";
        }
    } else {
        qDebug() << "Сокет уже подключен.";
    }
}

void LoginScreen::showLoginScreen()
{
    qDebug() << "Отображение экрана логина.";
    this->show();
}

void LoginScreen::openChatScreen(const QString &sessionID, const QString &userLogin)
{
    qDebug() << "Переход на экран чата с сессией:" << sessionID << "и логином пользователя:" << userLogin;
    this->hide();
    ChatScreen *chatScreen = new ChatScreen(sessionID, userLogin, socket);
    chatScreen->show();
    qDebug() << "Окно чата должно быть отображено.";

    disconnect(socket, &QTcpSocket::readyRead, this, &LoginScreen::onReadyRead);
}

void LoginScreen::saveSession(const QString &sessionID)
{
    if (sessionID.isEmpty()) {
        qDebug() << "Ошибка: попытка сохранить пустую сессию!";
        return;
    }

    QString filePath = "session.txt";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << sessionID;
        file.close();

        QFileInfo fileInfo(file);
        qDebug() << "Сессия успешно сохранена в файл:" << sessionID;
        qDebug() << "Путь к файлу сессии:" << fileInfo.absoluteFilePath();
    } else {
        qDebug() << "Ошибка открытия файла для записи сессии:" << file.errorString();
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить сессию. Проверьте доступ к файлу.");
    }
}

void LoginScreen::onReadyRead()
{
    QByteArray responseData = socket->readAll();
    buffer.append(responseData);

    while (buffer.contains('\n')) {
        int index = buffer.indexOf('\n');
        QByteArray singleMessage = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        if (singleMessage.isEmpty()) {
            continue;
        }

        if (singleMessage.isEmpty()) {
            qDebug() << "Пустое сообщение, пропускаем.";
            continue;
        }

        QJsonParseError parseError;
        QJsonDocument responseDoc = QJsonDocument::fromJson(singleMessage, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
            qDebug() << "Некорректный JSON:" << singleMessage;
            continue;
        }

        QJsonObject response = responseDoc.object();
        QString status = response.value("status").toString();
        QString messageText = response.value("message").toString();
        QString sessionID = response.value("sessionID").toString();
        QString login = response.value("login").toString();

        qDebug() << "Статус ответа:" << status << ", сообщение:" << messageText;

        if (status == "success") {
            if (!sessionID.isEmpty()) {
                qDebug() << "Сессия действительна, переход на экран чата с логином:" << login;
                saveSession(sessionID);

                openChatScreen(sessionID, login);
            }
        } else {
            QMessageBox::warning(this, "Ошибка", messageText);
        }
    }
}
