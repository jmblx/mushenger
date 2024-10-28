#ifndef CHATSCREEN_H
#define CHATSCREEN_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QSet>
#include <QMap>
#include <QImage>
#include <QListWidgetItem>
#include "chatitemdelegate.h"

namespace Ui {
class ChatScreen;
}

class ChatScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChatScreen(const QString &sessionID, const QString &userLogin, QTcpSocket *existingSocket, QWidget *parent = nullptr);
    ~ChatScreen();

private slots:
    void onChatDeleted(const QString &chatID);
    void onChatRenamed(const QString &chatID, const QString &newName);
    void onChatSelected(QListWidgetItem *item);
    void loadChats();
    void loadMessages(const QString &chatID);
    void onSendMessageClicked();
    void onReadyRead();
    void onEnterPressed();
    void onNewChatClicked();
    void onUserCheckCompleted(const QString &username, bool exists);
    void getUserData();
    void onAvatarClicked();
    void onThemeSwitchButtonClicked();
    void onThemeChanged(const QString& newTheme);
    void onGearIconClicked(const QModelIndex &index);
signals:
    void userCheckCompleted(const QString &username, bool exists);

private:
    QMap<QString, QPixmap> userAvatars;
    bool avatarSelected = false;

    ChatItemDelegate *chatItemDelegate;

    void deleteChat(const QString &chatID);
    void renameChat(const QString &chatID, const QString &newName);
    void updateChatListIcons();
    QMap<QString, QString> chatAdmins;
    void updateChatCount();
    QJsonObject avatarSyncData;
    void loadAvatarSyncData();
    void saveAvatarSyncData();
    Ui::ChatScreen *ui;
    QString pendingChatID;
    void sendUploadAvatarRequest();
    QTcpSocket *socket;
    QString sessionID;
    QString currentChatID;
    QString currentUserLogin;
    QImage currentUserAvatar;

    QSet<QString> validParticipants;
    QByteArray avatarData;
    QString chatName;

    QByteArray pendingAvatarData;
    QString pendingChatName;

    QMap<QString, QImage> chatAvatars;
    QMap<QString, QStringList> chatParticipants;

    QByteArray buffer;

    QString lastCheckedUsername;

    void updateUserAvatar(const QImage &image);
    void sendRequest(const QJsonObject &request);
    QByteArray processAvatar(const QString &filePath);
    void checkUserExists(const QString &username);

    void addParticipant();
    void createChat();
    void fetchChatAvatar(const QString &chatID);
    void displayChatAvatar(const QString &chatID, const QImage &avatar);

    QListWidgetItem* createChatListItem(const QString &chatName, const QString &chatID);
    void displayMessages(const QJsonArray &messages);
    void displayUserAvatar(const QImage &avatar);
    void requestAvatar(const QString &chatID);
    QPixmap makeCircularPixmap(const QPixmap &pixmap, int size) const;

    QMap<QString, QJsonObject> localMessageCache;

    void loadLocalCache();
    void saveLocalCache();
    void syncMessagesWithServer();
    void addMessageToCache(const QString &sender, const QString &text);
protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // CHATSCREEN_H
