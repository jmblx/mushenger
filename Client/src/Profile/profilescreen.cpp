#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"
#include "src/AccountExitDialog/AccountExitDialog.h"
#include "src/Chat/ChatScreen.h"
#include "src/Login/LoginScreen.h"
#include <QJsonDocument>
#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QMessageBox> // Added include for QMessageBox

ProfileScreen::ProfileScreen(const QString &sessionID, const QString &userLogin, QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfileScreen), socket(new QTcpSocket(this)), sessionID(sessionID), currentUserLogin(userLogin)
{
    ui->setupUi(this);

    connect(ui->overlayButton, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonClicked);
    connect(ui->overlayButton_arrow, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonArrowClicked);
    ui->loginLabel->setText(currentUserLogin);

    connectToServer();
    getUserData(sessionID);

    if (ui->logoutWidget && ui->overlayButton) {
        ui->overlayButton->setGeometry(ui->logoutWidget->geometry());
    }
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
    // No need to delete socket; it's parented to this class
}

void ProfileScreen::connectToServer()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost("127.0.0.1", 12345);
        if (!socket->waitForConnected(3000)) {
            qDebug() << "Ошибка: Не удалось подключиться к серверу.";
            QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу."); // Added QMessageBox
            qApp->exit();
        } else {
            qDebug() << "Успешное подключение к серверу.";
        }
    }
}

void ProfileScreen::getUserData(const QString &sessionID)
{
    QJsonObject request;
    request["type"] = "getUserData";
    request["sessionID"] = sessionID;
    sendRequest(request);
    qDebug() << "Запрос на получение данных пользователя отправлен.";
}

void ProfileScreen::sendRequest(const QJsonObject &request)
{
    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
    qDebug() << "Запрос отправлен на сервер:" << doc.toJson();
}

void ProfileScreen::showLoginScreen()
{
    LoginScreen *loginScreen = new LoginScreen();
    loginScreen->show();
    this->close();
}

void ProfileScreen::onOverlayButtonClicked()
{
    AccountExitDialog dialog(this);
    dialog.setModal(true);
    dialog.exec();
}

void ProfileScreen::onOverlayButtonArrowClicked()
{
    qDebug() << "Переход на экран чата с сессией:" << sessionID << " и логином:" << currentUserLogin;

    ChatScreen *chatScreen = new ChatScreen(sessionID, currentUserLogin);
    chatScreen->show();
    this->close();
}
