#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(const QString &sender, const QString &message, const QPixmap &avatarPixmap,
                           bool isTransparent, bool isCurrentUser, qint64 timestamp,
                           bool isDefaultAvatar, QWidget *parent = nullptr);

    QString getSender() const { return senderName; }

public slots:
    void onThemeChanged(const QString& newTheme);

private:
    QLabel *avatarLabel;
    QLabel *senderLabel;
    QLabel *messageLabel;
    QLabel *timestampLabel;
    QVBoxLayout *messageLayout;
    QHBoxLayout *mainLayout;

    QString senderName;
    bool isUsingDefaultAvatar;
};

#endif // MESSAGEWIDGET_H
