#include "messagewidget.h"
#include "src/ThemeManager/ThemeManager.h"
#include <QStyle>
#include <QPainter>
#include <QDateTime>

MessageWidget::MessageWidget(const QString &sender, const QString &message, const QPixmap &avatarPixmap,
                             bool isTransparent, bool isCurrentUser, qint64 timestamp,
                             bool isDefaultAvatar, QWidget *parent)
    : QWidget(parent), senderName(sender), isUsingDefaultAvatar(isDefaultAvatar)
{
    // Set up avatar
    avatarLabel = new QLabel(this);

    // Ensure the pixmap is square by cropping the larger dimension
    QPixmap squareAvatar = avatarPixmap;
    if (avatarPixmap.width() != avatarPixmap.height()) {
        int side = qMin(avatarPixmap.width(), avatarPixmap.height());
        squareAvatar = avatarPixmap.copy((avatarPixmap.width() - side) / 2, (avatarPixmap.height() - side) / 2, side, side);
    }

    // Scale the avatar to 40x40 without keeping aspect ratio to fill the circle completely
    QPixmap scaledAvatarPixmap = squareAvatar.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Create circular avatar using mask
    QPixmap circularAvatar(40, 40);
    circularAvatar.fill(Qt::transparent);

    QPainter painter(&circularAvatar);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(scaledAvatarPixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 40, 40);
    painter.end();

    // Apply mask to make the avatar perfectly circular
    circularAvatar.setMask(circularAvatar.createMaskFromColor(Qt::transparent));

    avatarLabel->setPixmap(circularAvatar);
    avatarLabel->setFixedSize(40, 40);

    // Set up sender label
    senderLabel = new QLabel(sender, this);
    QFont senderFont;
    senderFont.setBold(true);
    senderLabel->setFont(senderFont);
    senderLabel->setStyleSheet("QLabel { color: #555555; }");
    senderLabel->setAlignment(Qt::AlignLeft);

    // Set up message text
    messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setContentsMargins(10, 10, 10, 10);
    // Temporarily set a maximum width; it will be updated in resizeEvent
    messageLabel->setMaximumWidth(280); // 40 * 0.7 (assuming parent width is 400)

    // Set up timestamp label
    timestampLabel = new QLabel(this);
    QFont timestampFont;
    timestampFont.setPointSize(8);
    timestampLabel->setFont(timestampFont);
    timestampLabel->setStyleSheet("QLabel { color: #888888; }");
    timestampLabel->setAlignment(Qt::AlignRight);

    // Format timestamp
    QDateTime messageTime = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
    QString timeString = messageTime.toString("yyyy-MM-dd HH:mm:ss") + " UTC";
    timestampLabel->setText(timeString);

    // Set up message layout
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
        // Style for current user's messages
        messageLabel->setStyleSheet("QLabel {"
                                    "background-color: #28a745;"
                                    "color: white;"
                                    "border-radius: 15px;"
                                    "padding: 10px;"
                                    "}");
        avatarLabel->setVisible(!isTransparent);

        mainLayout->addStretch();
        mainLayout->addLayout(messageLayout);
        mainLayout->addWidget(avatarLabel);
    } else {
        // Style for other users' messages
        messageLabel->setStyleSheet("QLabel {"
                                    "background-color: #007bff;"
                                    "color: white;"
                                    "border-radius: 15px;"
                                    "padding: 10px;"
                                    "}");
        avatarLabel->setVisible(!isTransparent);

        mainLayout->addWidget(avatarLabel);
        mainLayout->addLayout(messageLayout);
        mainLayout->addStretch();
    }

    // Подключаем слот к сигналу изменения темы
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &MessageWidget::onThemeChanged);

    setLayout(mainLayout);
}

void MessageWidget::onThemeChanged(const QString& newTheme)
{
    if (isUsingDefaultAvatar) {
        QString defaultAvatarPath = QString(":/images/%1/profile-circled.svg").arg(newTheme);
        QPixmap defaultAvatar(defaultAvatarPath);

        // Создаём квадратную аватарку для сохранения пропорций
        QPixmap squareAvatar = defaultAvatar;
        if (defaultAvatar.width() != defaultAvatar.height()) {
            int side = qMin(defaultAvatar.width(), defaultAvatar.height());
            squareAvatar = defaultAvatar.copy((defaultAvatar.width() - side) / 2, (defaultAvatar.height() - side) / 2, side, side);
        }

        // Масштабируем аватарку до нужного размера
        QPixmap scaledAvatarPixmap = squareAvatar.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // Создаём круглую аватарку с помощью маски
        QPixmap circularAvatar(40, 40);
        circularAvatar.fill(Qt::transparent);

        QPainter painter(&circularAvatar);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(scaledAvatarPixmap));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 40, 40);
        painter.end();

        circularAvatar.setMask(circularAvatar.createMaskFromColor(Qt::transparent));

        // Обновляем QLabel с аватаркой
        avatarLabel->setPixmap(circularAvatar);
    }
}
