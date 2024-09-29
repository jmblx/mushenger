#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class Client;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

private slots:
    void onLoginButtonClicked();
    void onReadyRead();

private:
    Ui::Client *ui;
    QTcpSocket *socket;

    void sendLoginRequest(const QString &login, const QString &password);
    void saveSession(const QString &sessionID);
    void loadSession();
    void showLoginScreen();
    void getUserData(const QString &sessionID);
    void openChatScreen(const QString &sessionID, const QString &userLogin);
    void connectToServer();
};

#endif // LOGINSCREEN_H
