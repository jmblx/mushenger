#ifndef CHATSCREEN_H
#define CHATSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QJsonObject>    // Include for QJsonObject
#include <QJsonArray>     // Include for QJsonArray

namespace Ui {
class ChatScreen;
}

class ChatScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChatScreen(const QString &sessionID, const QString &userLogin, QWidget *parent = nullptr);
    ~ChatScreen();

private slots:
    void onChatSelected(QListWidgetItem *item);
    void loadChats();
    void loadMessages(const QString &chatID);
    void onSendMessageClicked();
    void onReadyRead();
    void onEnterPressed();
    void onNewChatClicked();
    bool checkUserExists(const QString &username);
    void sendAvatarToServer(const QByteArray &avatarData, const QString &chatName);
    void onAvatarClicked();

signals:
    void requestLogin();

private:
    Ui::ChatScreen *ui;
    QTcpSocket *socket;
    QString sessionID;
    QString currentChatID;
    QString currentUserLogin;

    // Added missing method declarations
    void connectToServer();
    void sendRequest(const QJsonObject &request);
    QByteArray processAvatar(const QString &filePath);
};

#endif // CHATSCREEN_H
