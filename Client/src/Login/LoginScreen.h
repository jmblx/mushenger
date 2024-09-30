#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
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

private:
    Ui::Client *ui;
    QTcpSocket *socket;
    QString sessionID;  // Added member variable to store sessionID

    void sendLoginRequest(const QString &login, const QString &password);
    void saveSession(const QString &sessionID);
    void loadSession();
    void showLoginScreen();
    void getUserData(const QString &sessionID);
    void openChatScreen(const QString &sessionID, const QString &userLogin);
    void connectToServer();

    bool profileRequested = false;  // Flag for profile request
    ProfileScreen *profileScreen = nullptr;  // Pointer to profile screen
};

#endif // LOGINSCREEN_H
