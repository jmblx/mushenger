#include "src/Login/LoginScreen.h"
#include "src/ThemeManager/ThemeManager.h"
#include "src/Profile/ProfileScreen.h"
#include <QDebug>
#include "LoginScreen.h"
#include "ui_LoginScreen.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "src/Chat/ChatScreen.h"
#include <QDebug>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(ui->LoginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &LoginScreen::onReadyRead);
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &LoginScreen::onThemeSwitchButtonClicked);

    // Подключение к ThemeManager
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &LoginScreen::onThemeChanged);
    // Применение текущей темы
    onThemeChanged(ThemeManager::instance().currentTheme());

    qDebug() << "Загрузка сессии при запуске...";
    loadSession();
}

LoginScreen::~LoginScreen()
{
    delete ui;
    delete socket;
}

void LoginScreen::onThemeSwitchButtonClicked()
{
    ThemeManager::instance().toggleTheme();
}

void LoginScreen::onThemeChanged(const QString& newTheme)
{
    // Обновление фонового изображения
    QString backgroundPath = QString(":/images/%1/auth_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);

    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Не удалось загрузить фоновое изображение:" << backgroundPath;
    }

    // Обновление других изображений или иконок, если необходимо
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

    connect(this, &LoginScreen::loginSuccess, this, [this](const QString &sessionID, const QString &login){
        if (profileRequested) {
            profileScreen = new ProfileScreen(sessionID, login);
            profileScreen->show();
        } else {
            openChatScreen(sessionID, login);
        }
    });
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
    socket->write(doc.toJson());
    socket->flush();

    qDebug() << "Запрос на вход отправлен:" << request;
}

void LoginScreen::loadSession()
{
    QFile file("session.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString sessionIDFromFile = in.readLine();
        if (!sessionIDFromFile.isEmpty()) {
            qDebug() << "Загружена сессия из файла:" << sessionIDFromFile;
            sessionID = sessionIDFromFile;  // Store in member variable
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
    socket->write(QJsonDocument(request).toJson());
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
    ChatScreen *chatScreen = new ChatScreen(sessionID, userLogin);
    chatScreen->show();
    qDebug() << "Окно чата должно быть отображено.";
}

void LoginScreen::onReadyRead()
{
    QByteArray response_data = socket->readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(response_data);

    if (!responseDoc.isObject()) {
        QMessageBox::critical(this, "Ошибка", "Неверный формат ответа от сервера.");
        qDebug() << "Неверный формат ответа от сервера.";
        return;
    }

    QJsonObject response = responseDoc.object();
    QString status = response.value("status").toString();
    QString message = response.value("message").toString();
    qDebug() << "Получен ответ от сервера:" << response;

    if (status == "success") {
        if (response.contains("sessionID")) {
            sessionID = response.value("sessionID").toString();
            if (sessionID.isEmpty()) {
                qDebug() << "Ошибка: sessionID пуст.";
            } else {
                saveSession(sessionID);
            }
        }

        QString login;
        if (response.contains("login")) {
            login = response.value("login").toString();
        } else if (response.contains("user")) {
            QJsonObject userObject = response.value("user").toObject();
            login = userObject.value("login").toString();
        }

        if (!sessionID.isEmpty() && !login.isEmpty()) {
            qDebug() << "Сессия действительна, переход на экран чата с логином:" << login;
            openChatScreen(sessionID, login);
            this->close();  // Close login window after successful login
            return;
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить данные сессии и логина.");
            qDebug() << "Не удалось получить данные сессии и логина.";
        }
    } else {
        QMessageBox::warning(this, "Ошибка", message);
        qDebug() << "Ошибка ответа от сервера:" << message;
    }
}

void LoginScreen::saveSession(const QString &sessionID)
{
    if (sessionID.isEmpty()) {
        qDebug() << "Ошибка: sessionID пуст, сохранение отменено.";
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
    }
}
