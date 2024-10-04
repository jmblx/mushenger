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
#include <QDebug>

ProfileScreen::ProfileScreen(const QString &sessionID, const QString &userLogin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen),
    socket(new QTcpSocket(this)),
    sessionID(sessionID),
    currentUserLogin(userLogin)
{
    ui->setupUi(this);
    ui->loginLabel->setText(currentUserLogin);

    // Приведение кнопок к AnimatedButton
    backButton = qobject_cast<AnimatedButton*>(ui->backButton);
    profileButton = qobject_cast<AnimatedButton*>(ui->profileButton);
    overlayButton = qobject_cast<AnimatedButton*>(ui->overlayButton);

    if (backButton) {
        // Настройка анимации для backButton
        backButton->setHoverAnimationProperty("iconSize", QSize(22, 22), QSize(26, 26), 200);
    }

    if (profileButton) {
        // Уменьшение размера иконки до половины
        profileButton->setHoverAnimationProperty("iconSize", QSize(80, 80), QSize(85, 85), 200);

        // Настройка эффекта размытия при наведении
        QString currentTheme = ThemeManager::instance().currentTheme();
        if (currentTheme == "dark") {
            profileButton->setHoverBlurEffect(QColor("#D8DCE4"), 10.0, 200);
            profileButton->setupOverlayLabel(":/images/light/plus_icon.svg");
        } else {
            profileButton->setHoverBlurEffect(QColor("#0D1321"), 10.0, 200);
            profileButton->setupOverlayLabel(":/images/dark/plus_icon.svg");
        }
    }


    if (overlayButton) {
        // Подключение сигналов hover
        connect(overlayButton, &AnimatedButton::hoverEntered, this, &ProfileScreen::onOverlayButtonHoverEntered);
        connect(overlayButton, &AnimatedButton::hoverLeft, this, &ProfileScreen::onOverlayButtonHoverLeft);
    }

    // Подключение сигналов и слотов
    connect(ui->overlayButton, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &ProfileScreen::onBackButtonClicked);

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

    if (profileButton) {
        // Уменьшение размера иконки до половины
        profileButton->setHoverAnimationProperty("iconSize", QSize(80, 80), QSize(85, 85), 200);

        QString currentTheme = ThemeManager::instance().currentTheme();
        QString profilePlusIcon = QString(":/images/%1/avatar_plus_icon.svg").arg(currentTheme);
        profileButton->setupOverlayLabel(profilePlusIcon);

        if (ui->profileIcon) {
            QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(ui->profileIcon);
            blurEffect->setBlurRadius(0.0);
            ui->profileIcon->setGraphicsEffect(blurEffect);

            QPropertyAnimation *blurEnterAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
            blurEnterAnimation->setDuration(200);
            blurEnterAnimation->setStartValue(0.0);
            blurEnterAnimation->setEndValue(10.0);

            QPropertyAnimation *blurLeaveAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
            blurLeaveAnimation->setDuration(200);
            blurLeaveAnimation->setStartValue(10.0);
            blurLeaveAnimation->setEndValue(0.0);

            // Используем методы доступа для анимаций
            profileButton->getHoverEnterAnimation()->addAnimation(blurEnterAnimation);
            profileButton->getHoverLeaveAnimation()->addAnimation(blurLeaveAnimation);
        }
    }
}

void ProfileScreen::connectToServer()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost("127.0.0.1", 12345);
        if (!socket->waitForConnected(3000)) {
            qDebug() << "Ошибка: Не удалось подключиться к серверу.";
            QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу.");
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

void ProfileScreen::onBackButtonClicked()
{
    qDebug() << "Переход на экран чата с сессией:" << sessionID << " и логином:" << currentUserLogin;

    ChatScreen *chatScreen = new ChatScreen(sessionID, currentUserLogin);
    chatScreen->show();
    this->close();
}

void ProfileScreen::onOverlayButtonHoverEntered()
{
    QString currentTheme = ThemeManager::instance().currentTheme();
    QIcon hoverIcon(QString(":/images/%1/door_open.svg").arg(currentTheme));
    ui->logoutButtonIcon->setIcon(hoverIcon);
}

void ProfileScreen::onOverlayButtonHoverLeft()
{
    QString currentTheme = ThemeManager::instance().currentTheme();
    QIcon normalIcon(QString(":/images/%1/door.svg").arg(currentTheme));
    ui->logoutButtonIcon->setIcon(normalIcon);
}
