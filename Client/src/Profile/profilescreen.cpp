#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"
#include "src/AccountExitDialog/AccountExitDialog.h"
#include "src/ThemeManager/ThemeManager.h"
#include "src/Chat/ChatScreen.h"
#include "src/Login/LoginScreen.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QMessageBox>

ProfileScreen::ProfileScreen(const QString &sessionID, const QString &userLogin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen),
    socket(new QTcpSocket(this)),
    sessionID(sessionID),
    currentUserLogin(userLogin)
{
    ui->setupUi(this);
    ui->loginLabel->setText(currentUserLogin);

    connect(ui->overlayButton, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonClicked);
    connect(ui->overlayButton_arrow, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonArrowClicked);

    // Подключение к ThemeManager
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &ProfileScreen::onThemeChanged);
    // Применение текущей темы
    onThemeChanged(ThemeManager::instance().currentTheme());
    // Подключение кнопки переключения темы (если есть)
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &ProfileScreen::onThemeSwitchButtonClicked);

    connectToServer();
    getUserData(sessionID);
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
    // No need to delete socket; it's parented to this class
}

void ProfileScreen::onThemeSwitchButtonClicked()
{
    ThemeManager::instance().toggleTheme();
}

void ProfileScreen::onThemeChanged(const QString& newTheme)
{
    QString backgroundPath = QString(":/images/%1/profile_screen.png").arg(newTheme);
    QPixmap backgroundPixmap(backgroundPath);
    if (!backgroundPixmap.isNull()) {
        ui->background->setPixmap(backgroundPixmap);
    } else {
        qDebug() << "Failed to load background image:" << backgroundPath;
    }

    QIcon doorIcon(QString(":/images/%1/door.svg").arg(newTheme));
    ui->logoutButtonIcon->setIcon(doorIcon);

    QIcon backIcon(QString(":/images/%1/out_pointer.svg").arg(newTheme));
    ui->backButton->setIcon(backIcon);

    QString profileIconPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
    QPixmap profileIcon(profileIconPath);
    if (!profileIcon.isNull()) {
        ui->profileIcon->setPixmap(profileIcon);
    } else {
        qDebug() << "Failed to load profile icon:" << profileIconPath;
    }
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
