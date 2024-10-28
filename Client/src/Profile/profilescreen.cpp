#include "ProfileScreen.h"
#include "ui_ProfileScreen.h"
#include "src/AccountExitDialog/AccountExitDialog.h"
#include "src/AnimatedComboBox/AnimatedComboBox.h"
#include "src/AnimatedSpinBox/AnimatedSpinBox.h"
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
#include <QFileDialog>
#include <QPainter>
#include <QFontDatabase>
#include <QSettings>
#include <QJsonObject>
#include <QRegularExpression>

ProfileScreen::ProfileScreen(const QString &sessionID, const QString &userLogin, QTcpSocket *existingSocket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileScreen),
    socket(existingSocket),
    sessionID(sessionID),
    currentUserLogin(userLogin)
{
    ui->setupUi(this);
    ui->loginLabel->setText(currentUserLogin);

    QString savedFontFamily = ThemeManager::instance().fontFamily();
    int savedFontScale = ThemeManager::instance().fontScale();

    QFontDatabase fontDatabase;
    ui->fontComboBox->addItems(fontDatabase.families());
    ui->fontComboBox->setCurrentText(savedFontFamily);

    ui->fontSizeSpinBox->setRange(1, 200);
    ui->fontSizeSpinBox->setValue(savedFontScale);

    backButton = qobject_cast<AnimatedButton*>(ui->backButton);
    profileButton = qobject_cast<AnimatedButton*>(ui->profileButton);
    overlayButton = qobject_cast<AnimatedButton*>(ui->overlayButton);

    if (backButton) {
        backButton->setHoverAnimationProperty("iconSize", QSize(22, 22), QSize(26, 26), 200);
    }

    if (profileButton) {
        profileButton->setHoverAnimationProperty("iconSize", QSize(80, 80), QSize(85, 85), 200);
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
        connect(overlayButton, &AnimatedButton::hoverEntered, this, &ProfileScreen::onOverlayButtonHoverEntered);
        connect(overlayButton, &AnimatedButton::hoverLeft, this, &ProfileScreen::onOverlayButtonHoverLeft);
    }

    connect(ui->overlayButton, &QPushButton::clicked, this, &ProfileScreen::onOverlayButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &ProfileScreen::onBackButtonClicked);
    connect(ui->profileButton, &QPushButton::clicked, this, &ProfileScreen::onProfileButtonClicked);
    connect(socket, &QTcpSocket::readyRead, this, &ProfileScreen::onReadyRead);
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &ProfileScreen::onThemeChanged);
    onThemeChanged(ThemeManager::instance().currentTheme());
    connect(ui->themeSwitchButton, &QPushButton::clicked, this, &ProfileScreen::onThemeSwitchButtonClicked);

    connect(ui->fontComboBox, &QComboBox::currentTextChanged, &ThemeManager::instance(), &ThemeManager::setFontFamily);
    connect(ui->fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), &ThemeManager::instance(), &ThemeManager::setFontScale);

    QObject::disconnect(socket, nullptr, nullptr, nullptr);

    connect(socket, &QTcpSocket::readyRead, this, &ProfileScreen::onReadyRead);

    loadAvatarSyncData();
    getUserData();
    analyzePassword();
}

ProfileScreen::~ProfileScreen()
{
    delete ui;
}

void ProfileScreen::analyzePassword()
{
    QString password = userPassword;
    QStringList issues;

    if (password.length() < 8) {
        issues << "Меньше 8 символов!";
    }

    QRegularExpression uppercaseRegex("[A-Z]");
    if (!password.contains(uppercaseRegex)) {
        issues << "В Вашем пароле нет заглавных букв!";
    }

    QRegularExpression lowercaseRegex("[a-z]");
    if (!password.contains(lowercaseRegex)) {
        issues << "В Вашем пароле нет строчных букв!";
    }

    QRegularExpression digitRegex("[0-9]");
    if (!password.contains(digitRegex)) {
        issues << "В Вашем пароле нет цифр!";
    }

    QRegularExpression specialCharRegex("[^a-zA-Z0-9]");
    if (!password.contains(specialCharRegex)) {
        issues << "В Вашем пароле нет спецсимволов!";
    }

    if (password.contains(" ")) {
        issues << "В Вашем пароле есть пробелы!";
    }
    if (issues.isEmpty()) {
        ui->mushroomMessage->setText("Ваш пароль безопасен! ✅");
    } else {
        ui->mushroomMessage->setText("Ваш пароль не безопасен! ⚠️");
    }
}

void ProfileScreen::loadAvatarSyncData()
{
    QFile file("avatar_sync.json");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                avatarSyncData = doc.object();
                qDebug() << "Данные синхронизации аватаров загружены.";
            } else {
                qDebug() << "Ошибка парсинга avatar_sync.json.";
            }
            file.close();
        } else {
            qDebug() << "Не удалось открыть avatar_sync.json для чтения.";
        }
    } else {
        qDebug() << "Файл avatar_sync.json не найден. Начинаем с пустых данных синхронизации.";
        avatarSyncData = QJsonObject();
    }
}

void ProfileScreen::saveAvatarSyncData()
{
    QFile file("avatar_sync.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(avatarSyncData);
        file.write(doc.toJson());
        file.close();
        qDebug() << "Данные синхронизации аватаров сохранены.";
    } else {
        qDebug() << "Не удалось сохранить avatar_sync.json.";
    }
}

void ProfileScreen::getUserData()
{
    QJsonObject request;
    request["type"] = "getUserData";
    request["sessionID"] = sessionID;

    qint64 lastSync = 0;
    if (avatarSyncData.contains(currentUserLogin)) {
        lastSync = avatarSyncData[currentUserLogin].toInt();
    }
    request["avatarLastUpdate"] = lastSync;

    sendRequest(request);
    qDebug() << "Запрос на получение данных пользователя отправлен.";
}

void ProfileScreen::onLogout()
{
    qDebug() << "Пользователь вышел из аккаунта. Переход на экран входа.";
    this->close();
    LoginScreen *loginScreen = new LoginScreen();
    loginScreen->show();
}

void ProfileScreen::onReadyRead()
{
    QByteArray responseData = socket->readAll();
    //qDebug() << "Полученные данные от сервера:" << responseData;

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

        qDebug() << "Статус ответа:" << status << ", сообщение:" << messageText;
        if (response.contains("type")) {
            QString responseType = response.value("type").toString();
            qDebug() << "Тип ответа:" << responseType;

            if (responseType == "userData") {
                QJsonObject userObj = response["user"].toObject();
                QString avatarBase64 = userObj.value("avatar").toString();
                qint64 serverLastUpdate = userObj.value("avatarLastUpdated").toInt();

                userPassword = userObj.value("password").toString();

                if (!avatarBase64.isEmpty()) {
                    QByteArray avatarData = QByteArray::fromBase64(avatarBase64.toUtf8());
                    QImage avatarImage;
                    avatarImage.loadFromData(avatarData);
                    displayUserAvatar(avatarImage);
                    qDebug() << "Аватар пользователя успешно загружен и отображен.";

                    avatarSyncData[currentUserLogin] = serverLastUpdate;
                    saveAvatarSyncData();
                } else {
                    qDebug() << "Аватар пользователя отсутствует. Устанавливаем аватар по умолчанию.";
                    QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(ThemeManager::instance().currentTheme());
                    QPixmap defaultAvatar(defaultAvatarPath);
                    if (!defaultAvatar.isNull()) {
                        ui->profileIcon->setPixmap(defaultAvatar);
                    }
                }
            }
            else if (responseType == "uploadProfileAvatar") {
                if (messageText == "Avatar uploaded successfully.") {
                    getUserData();
                    qDebug() << "Аватар успешно загружен. Запрашиваем обновлённые данные пользователя.";
                }
            }
            else {
                qDebug() << "Неизвестный тип ответа:" << responseType;
            }
        }
        else {
            qDebug() << "Получено сообщение без поля 'type'.";
        }
    }
    analyzePassword();
}

void ProfileScreen::onProfileButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите аватарку", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        QImage image(filePath);
        if (!image.isNull()) {
            QImage scaledImage = image.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            scaledImage.save(&buffer, "PNG");

            QString avatarBase64 = QString(imageData.toBase64());

            QJsonObject avatarRequest;
            avatarRequest["type"] = "uploadProfileAvatar";
            avatarRequest["sessionID"] = sessionID;
            avatarRequest["avatar"] = avatarBase64;

            sendRequest(avatarRequest);

            displayUserAvatar(scaledImage);
            qDebug() << "Аватарка обновлена в интерфейсе.";
        } else {
            QMessageBox::warning(this, "Ошибка", "Невозможно загрузить изображение.");
        }
    }
}

void ProfileScreen::displayUserAvatar(const QImage &avatar)
{
    currentUserAvatar = avatar;

    QImage scaledImage = avatar.scaled(161, 161, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap pixmap = QPixmap::fromImage(scaledImage);

    QPixmap circularPixmap(161, 161);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 161, 161);

    ui->profileIcon->setPixmap(circularPixmap);
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

    if (!currentUserAvatar.isNull()) {
        displayUserAvatar(currentUserAvatar);
    } else {
        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
        QPixmap defaultAvatar(defaultAvatarPath);
        if (!defaultAvatar.isNull()) {
            ui->profileIcon->setPixmap(defaultAvatar);
        } else {
            qDebug() << "Failed to load default avatar image:" << defaultAvatarPath;
        }
    }

    if (profileButton) {
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

            profileButton->getHoverEnterAnimation()->addAnimation(blurEnterAnimation);
            profileButton->getHoverLeaveAnimation()->addAnimation(blurLeaveAnimation);
        }
    }
}

void ProfileScreen::sendRequest(const QJsonObject &request)
{
    QJsonDocument doc(request);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n";
    qDebug() << "Отправка запроса на сервер:" << jsonData;

    socket->write(jsonData);
    socket->flush();
}

void ProfileScreen::onOverlayButtonClicked()
{
    AccountExitDialog *dialog = new AccountExitDialog(sessionID, this);
    connect(dialog, &AccountExitDialog::logoutSuccessful, this, &ProfileScreen::onLogout);
    dialog->setModal(true);
    dialog->exec();
    dialog->deleteLater();
}

void ProfileScreen::onBackButtonClicked()
{
    qDebug() << "Переход на экран чата с сессией:" << sessionID << " и логином:" << currentUserLogin;

    QObject::disconnect(socket, nullptr, nullptr, nullptr);

    ChatScreen *chatScreen = new ChatScreen(sessionID, currentUserLogin, socket);
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
