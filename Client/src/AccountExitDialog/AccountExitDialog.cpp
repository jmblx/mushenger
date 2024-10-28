#include "AccountExitDialog.h"
#include "ui_AccountExitDialog.h"
#include "src/ThemeManager/ThemeManager.h"
#include "src/Login/LoginScreen.h"
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFile>
#include <QDir>

AccountExitDialog::AccountExitDialog(const QString &sessionID, QWidget *parent)
    : QDialog(parent),
    currentSessionID(sessionID),
    ui(new Ui::AccountExitDialog)
{
    ui->setupUi(this);
    setModal(true);
    ui->yesButton->raise();
    ui->yesButton->setText("Да");
    ui->noButton->raise();


    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &AccountExitDialog::onThemeChanged);
    onThemeChanged(ThemeManager::instance().currentTheme());

    connect(ui->yesButton, &QPushButton::clicked, this, &AccountExitDialog::onYesButtonClicked);
    connect(ui->noButton, &QPushButton::clicked, this, &QDialog::reject);
}

void AccountExitDialog::onYesButtonClicked()
{
    qDebug() << "Кнопка 'Да' нажата. Начинается процесс выхода из аккаунта с currentSessionID: " << currentSessionID;

    ui->yesButton->setEnabled(false);
    ui->noButton->setEnabled(false);

    QTcpSocket *socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, [this, socket]() {
        QJsonObject logoutRequest;
        logoutRequest["type"] = "logout";
        logoutRequest["sessionID"] = currentSessionID;

        qDebug() << "Отправка запроса на выход: " << QJsonDocument(logoutRequest).toJson(QJsonDocument::Compact);

        socket->write(QJsonDocument(logoutRequest).toJson(QJsonDocument::Compact) + "\n");
        socket->flush();
    });

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray responseData = socket->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
        QJsonObject response = responseDoc.object();

        if (response.value("status").toString() == "success") {
            qDebug() << "Выход из аккаунта успешен. Очистка данных и переход на экран логина.";
            this->clearUserData();
            emit logoutSuccessful();
            accept();
        } else {
            qDebug() << "Ошибка выхода из аккаунта: " << response.value("message").toString();
            QMessageBox::warning(this, "Ошибка", "Не удалось выйти из аккаунта.");

            ui->yesButton->setEnabled(true);
            ui->noButton->setEnabled(true);
        }

        socket->disconnectFromHost();
        socket->deleteLater();
    });

    connect(socket, &QTcpSocket::errorOccurred, [this, socket](QAbstractSocket::SocketError socketError){
        qDebug() << "Ошибка сокета при выходе из аккаунта:" << socketError;
        QMessageBox::warning(this, "Ошибка сети", "Не удалось подключиться к серверу.");

        ui->yesButton->setEnabled(true);
        ui->noButton->setEnabled(true);

        socket->disconnectFromHost();
        socket->deleteLater();
    });

    qDebug() << "Подключение к серверу для выхода из аккаунта.";
    socket->connectToHost("127.0.0.1", 12345);
}


AccountExitDialog::~AccountExitDialog()
{
    delete ui;
}

void AccountExitDialog::clearUserData() {
    qDebug() << "Очистка данных сессии и пользователя.";

    currentSessionID.clear();
    userData = QJsonObject();

    QString sessionFilePath = QDir::currentPath() + "/session.txt";
    if (QFile::exists(sessionFilePath)) {
        QFile::remove(sessionFilePath);
        qDebug() << "Файл сессии удалён:" << sessionFilePath;
    }

    QString cacheFilePath = QDir::currentPath() + "/message_cache.json";
    if (QFile::exists(cacheFilePath)) {
        QFile::remove(cacheFilePath);
        qDebug() << "Локальный кэш сообщений удалён:" << cacheFilePath;
    }

    qDebug() << "Данные сессии и пользователя очищены.";
}


void AccountExitDialog::showLoginScreen() {
    qDebug() << "Переход на экран логина.";
    LoginScreen *loginScreen = new LoginScreen();
    loginScreen->show();
    QApplication::exit();
    qDebug() << "Экран логина открыт.";
}

void AccountExitDialog::onThemeChanged(const QString& newTheme)
{
    QString backgroundPath = QString(":/images/%1/account_exit_dialog_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);
    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Failed to load background image:" << backgroundPath;
    }
}
