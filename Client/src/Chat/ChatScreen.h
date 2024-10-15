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
    void onChatDeleted(const QString &chatID);  // Слот для обработки удаления чата
    void onChatRenamed(const QString &chatID, const QString &newName);
    void onChatSelected(QListWidgetItem *item); // Слот для обработки выбора чата
    void loadChats();                          // Загрузка чатов с сервера
    void loadMessages(const QString &chatID);  // Загрузка сообщений для выбранного чата
    void onSendMessageClicked();               // Обработка нажатия кнопки отправки сообщения
    void onReadyRead();                        // Обработка полученных данных от сервера
    void onEnterPressed();                     // Слот для обработки нажатия Enter
    void onNewChatClicked();                   // Слот для создания нового чата
    void onUserCheckCompleted(const QString &username, bool exists);    // Слот для обработки завершения проверки пользователя
    void getUserData();
    void onAvatarClicked();
    void onThemeSwitchButtonClicked();
    void onThemeChanged(const QString& newTheme); // Слот для обновления UI при смене темы
    void onGearIconClicked(const QModelIndex &index);

signals:
    void userCheckCompleted(const QString &username, bool exists);

private:
    bool avatarSelected = false;  // Добавим в класс ChatScreen

    ChatItemDelegate *chatItemDelegate;  // Добавляем переменную для делегата чатов

    void deleteChat(const QString &chatID);     // Функция для отправки запроса на удаление чата
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
    QTcpSocket *socket;          // Используемый сокет
    QString sessionID;           // Идентификатор сессии
    QString currentChatID;       // Идентификатор текущего чата
    QString currentUserLogin;    // Текущий логин пользователя
    QImage currentUserAvatar;

    QSet<QString> validParticipants; // Множество валидных участников чата (без дубликатов)
    QByteArray avatarData;        // Данные аватара для нового чата
    QString chatName;             // Название нового чата

    QByteArray pendingAvatarData; // Данные аватара, ожидающие отправки
    QString pendingChatName;      // Название чата, для которого ожидается загрузка аватара

    QMap<QString, QImage> chatAvatars;
    QMap<QString, QStringList> chatParticipants;

    QByteArray buffer;            // Буфер для поступающих данных

    QString lastCheckedUsername;  // Последний проверяемый участник

    // Методы
    void updateUserAvatar(const QImage &image);
    void sendRequest(const QJsonObject &request);   // Отправка JSON-запроса на сервер
    QByteArray processAvatar(const QString &filePath); // Обработка аватарного изображения
    void checkUserExists(const QString &username);

    void addParticipant();                           // Добавление участника в чат
    void createChat();                               // Создание чата после проверки всех участников
    void fetchChatAvatar(const QString &chatID);     // Запрос аватара для чата
    void displayChatAvatar(const QString &chatID, const QImage &avatar);   // Отображение аватара в интерфейсе

    // Вспомогательные методы
    QListWidgetItem* createChatListItem(const QString &chatName, const QString &chatID); // Создание элемента списка чатов с аватаром
    void displayMessages(const QJsonArray &messages); // Отображение сообщений в интерфейсе
    void displayUserAvatar(const QImage &avatar);
    void requestAvatar(const QString &chatID);

    QMap<QString, QJsonObject> localMessageCache; // Ключ: chatID, значение: объект с сообщениями и последним timestamp

    // Новые методы
    void loadLocalCache();
    void saveLocalCache();
    void syncMessagesWithServer();
    void addMessageToCache(const QString &sender, const QString &text);
protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // CHATSCREEN_H
