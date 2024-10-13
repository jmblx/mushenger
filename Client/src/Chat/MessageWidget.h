#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(const QString &sender, const QString &message, const QString &avatarPath,
                           bool isTransparent, bool isCurrentUser, qint64 timestamp, QWidget *parent = nullptr);

    QString getSender() const { return senderName; }

private:
    QLabel *avatarLabel;
    QLabel *senderLabel;    // Новая метка для логина отправителя
    QLabel *messageLabel;
    QLabel *timestampLabel; // Новая метка для времени
    QHBoxLayout *mainLayout;
    QVBoxLayout *messageLayout;

    QString senderName;
};

#endif // MESSAGEWIDGET_H
