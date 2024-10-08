#ifndef PROFILESCREEN_H
#define PROFILESCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include "src/AnimatedButton/AnimatedButton.h"

namespace Ui {
class ProfileScreen;
}

class ProfileScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileScreen(const QString &sessionID, const QString &userLogin, QWidget *parent = nullptr);
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
    void updateProfileIcon(const QImage &image);
    void onLogout();

signals:
    void sendRequestToServer(const QByteArray &request);

private:
    Ui::ProfileScreen *ui;
    QTcpSocket *socket;
    QString sessionID;
    QString currentUserLogin;
    QImage currentUserAvatar;

    AnimatedButton *profileButton;
    AnimatedButton *backButton;
    AnimatedButton *overlayButton;

    void connectToServer();
    void loadSession();
    void showLoginScreen();
    void getUserData(const QString &sessionID);
    void sendRequest(const QJsonObject &request);
};

#endif // PROFILESCREEN_H
