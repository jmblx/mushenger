#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include "src/Profile/ProfileScreen.h"

namespace Ui {
class Client;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

signals:
    void loginSuccess(const QString &sessionID, const QString &login);

private slots:
    void onLoginButtonClicked();
    void onReadyRead();
    void onThemeSwitchButtonClicked();
    void onThemeChanged(const QString& newTheme); // Слот для обновления UI при смене темы

private:
    Ui::Client *ui;
    QTcpSocket *socket;
    QString sessionID;

    void sendLoginRequest(const QString &login, const QString &password);
    void saveSession(const QString &sessionID);
    void loadSession();
    void showLoginScreen();
    void getUserData(const QString &sessionID);
    void openChatScreen(const QString &sessionID, const QString &userLogin);
    void connectToServer();

    bool profileRequested = false;
    ProfileScreen *profileScreen = nullptr;
};

#endif // LOGINSCREEN_H
