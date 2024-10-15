#include "messagewidget.h"
#include <QStyle>
#include <QPainter>
#include <QDateTime>

MessageWidget::MessageWidget(const QString &sender, const QString &message, const QString &avatarPath,
                             bool isTransparent, bool isCurrentUser, qint64 timestamp, QWidget *parent)
    : QWidget(parent), senderName(sender)
{
    // Настройка аватара
    avatarLabel = new QLabel(this);
    QPixmap avatarPixmap(avatarPath);
    avatarPixmap = avatarPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // Создание круглого аватара
    QPixmap circularAvatar(40, 40);
    circularAvatar.fill(Qt::transparent);
    QPainter painter(&circularAvatar);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(avatarPixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 40, 40);
    painter.end();
    avatarLabel->setPixmap(circularAvatar);
    avatarLabel->setFixedSize(40, 40);

    // Настройка метки логина отправителя
    senderLabel = new QLabel(sender, this);
    QFont senderFont;
    senderFont.setBold(true);
    senderLabel->setFont(senderFont);
    senderLabel->setStyleSheet("QLabel { color: #555555; }"); // Серый цвет
    senderLabel->setAlignment(Qt::AlignLeft);

    // Настройка текста сообщения
    messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setContentsMargins(10, 10, 10, 10);
    // Устанавливаем максимальную ширину сообщения (70% родительского списка)
    messageLabel->setMaximumWidth(this->parentWidget()->width() * 0.7);

    // Настройка метки времени
    timestampLabel = new QLabel(this);
    QFont timestampFont;
    timestampFont.setPointSize(8); // Маленький размер шрифта
    timestampLabel->setFont(timestampFont);
    timestampLabel->setStyleSheet("QLabel { color: #888888; }"); // Светло-серый цвет
    timestampLabel->setAlignment(Qt::AlignRight);

    // Форматирование времени из timestamp
    QDateTime messageTime = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
    QString timeString = messageTime.toString("yyyy-MM-dd HH:mm:ss") + " UTC";
    timestampLabel->setText(timeString);

    // Настройка компоновки сообщения
    messageLayout = new QVBoxLayout();
    messageLayout->setContentsMargins(0, 0, 0, 0);
    messageLayout->setSpacing(2);
    messageLayout->addWidget(senderLabel);
    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(timestampLabel);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);

    if (isCurrentUser) {
        // Стиль для сообщений текущего пользователя
        messageLabel->setStyleSheet("QLabel {"
                                    "background-color: #28a745;" // Насыщенный зелёный фон
                                    "color: white;"              // Белый текст для контраста
                                    "border-radius: 15px;"
                                    "padding: 10px;"
                                    "}");
        if (isTransparent) {
            avatarLabel->setVisible(false);
        } else {
            avatarLabel->setVisible(true);
        }

        mainLayout->addStretch();
        mainLayout->addLayout(messageLayout);
        mainLayout->addWidget(avatarLabel);
    }
    else {
        // Стиль для сообщений других пользователей
        messageLabel->setStyleSheet("QLabel {"
                                    "background-color: #007bff;" // Насыщенный синий фон
                                    "color: white;"              // Белый текст для контраста
                                    "border-radius: 15px;"
                                    "padding: 10px;"
                                    "}");
        if (isTransparent) {
            avatarLabel->setVisible(false);
        } else {
            avatarLabel->setVisible(true);
        }

        mainLayout->addWidget(avatarLabel);
        mainLayout->addLayout(messageLayout);
        mainLayout->addStretch();
    }

    setLayout(mainLayout);
}
