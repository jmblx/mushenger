#ifndef PROFILESCREEN_H
#define PROFILESCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include "src/AnimatedButton/AnimatedButton.h"
#include <QJsonObject>
#include <QImage>

namespace Ui {
class ProfileScreen;
}

class ProfileScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileScreen(const QString &sessionID, const QString &userLogin, QTcpSocket *existingSocket, QWidget *parent = nullptr);
    ~ProfileScreen();

private slots:
    void onOverlayButtonClicked();
    void onBackButtonClicked();
    void onThemeSwitchButtonClicked();
    void onThemeChanged(const QString& newTheme);
    void onReadyRead();
    void onOverlayButtonHoverEntered();
    void onOverlayButtonHoverLeft();
    void onProfileButtonClicked();
    void onLogout();
    void displayUserAvatar(const QImage &avatar); // Аналогично ChatScreen

private:
    Ui::ProfileScreen *ui;
    QTcpSocket *socket;
    QString sessionID;
    QString currentUserLogin;
    QImage currentUserAvatar; // Переменная для хранения аватара пользователя

    AnimatedButton *profileButton;
    AnimatedButton *backButton;
    AnimatedButton *overlayButton;

    // Данные синхронизации аватаров
    QJsonObject avatarSyncData;
    void loadAvatarSyncData();
    void saveAvatarSyncData();

    void sendRequest(const QJsonObject &request);
    void getUserData();
    QByteArray buffer; // Буфер для чтения данных от сервера
};

#endif // PROFILESCREEN_H
