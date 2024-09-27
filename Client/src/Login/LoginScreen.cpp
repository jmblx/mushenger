#include "src/Login/LoginScreen.h"
#include "src/Login/ui_LoginScreen.h"
#include <QDebug>
#include "LoginScreen.h"
#include "ui_LoginScreen.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(ui->LoginButton, &QPushButton::clicked, this, &LoginScreen::onLoginButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &LoginScreen::onReadyRead);

    loadSession();
}

LoginScreen::~LoginScreen()
{
    delete ui;
    delete socket;
}

void LoginScreen::onLoginButtonClicked()
{
    QString login = ui->LoginInput->text();
    QString password = ui->PwdInput->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    sendLoginRequest(login, password);
}

void LoginScreen::sendLoginRequest(const QString &login, const QString &password)
{
    socket->connectToHost("127.0.0.1", 12345);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
        return;
    }

    QJsonObject request;
    request["login"] = login;
    request["password"] = password;

    QJsonDocument doc(request);
    socket->write(doc.toJson());
}

void LoginScreen::onReadyRead()
{
    QByteArray response_data = socket->readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(response_data);

    if (!responseDoc.isObject()) {
        QMessageBox::critical(this, "Ошибка", "Неверный формат ответа от сервера.");
        return;
    }

    QJsonObject response = responseDoc.object();
    QString status = response.value("status").toString();
    QString message = response.value("message").toString();
    QString sessionID = response.value("sessionID").toString();

    if (status == "success") {
        QMessageBox::information(this, "Успех", message);
        if (!sessionID.isEmpty()) {
            saveSession(sessionID);
        }
    } else {
        QMessageBox::warning(this, "Ошибка", message);
    }
}

void LoginScreen::saveSession(const QString &sessionID)
{
    QFile file("session.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << sessionID;
    }
}

void LoginScreen::loadSession()
{
    QFile file("session.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString sessionID = in.readLine();
        if (!sessionID.isEmpty()) {
            // Отправляем запрос на сервер для проверки сессии
            QJsonObject request;
            request["sessionID"] = sessionID;
            QJsonDocument doc(request);

            socket->connectToHost("127.0.0.1", 12345);
            if (socket->waitForConnected(3000)) {
                socket->write(doc.toJson());
                socket->flush();
            }
        } else {
            showLoginScreen();
        }
    } else {
        showLoginScreen();
    }
}

void LoginScreen::showLoginScreen()
{
    this->show();
}

