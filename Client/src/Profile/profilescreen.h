#ifndef PROFILESCREEN_H
#define PROFILESCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>    // Added include for QJsonObject
#include <QMessageBox>    // Added include for QMessageBox if used

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
    void onOverlayButtonArrowClicked();

private:
    Ui::ProfileScreen *ui;
    QTcpSocket *socket;
    QString sessionID;
    QString currentUserLogin;

    void connectToServer();
    void loadSession();
    void showLoginScreen();
    void getUserData(const QString &sessionID);
    void sendRequest(const QJsonObject &request);
};

#endif // PROFILESCREEN_H
