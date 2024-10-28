#include "chatitemdelegate.h"
#include "src/ThemeManager/ThemeManager.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAbstractItemView>

ChatItemDelegate::ChatItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &ChatItemDelegate::onThemeChanged);

    QString currentTheme = ThemeManager::instance().currentTheme();
    gearIcon = QIcon(QString(":/images/%1/gear.svg").arg(currentTheme));

    if (gearIcon.isNull()) {
        qDebug() << "Не удалось загрузить иконку шестерёнки.";
    } else {
        qDebug() << "Иконка шестерёнки успешно загружена.";
    }
}

void ChatItemDelegate::onThemeChanged(const QString& newTheme)
{
    gearIcon = QIcon(QString(":/images/%1/gear.svg").arg(newTheme));
    if (gearIcon.isNull()) {
        qDebug() << "Не удалось загрузить иконку шестерёнки для темы:" << newTheme;
    } else {
        qDebug() << "Иконка шестерёнки обновлена для темы:" << newTheme;
    }

    if (QAbstractItemView* view = qobject_cast<QAbstractItemView*>(parent())) {
        view->viewport()->update();
    }
}

void ChatItemDelegate::setAdminChats(const QMap<QString, QString> &adminChatsMap)
{
    adminChats = adminChatsMap;
    qDebug() << "Список чатов с администраторами:";
    QMapIterator<QString, QString> it(adminChats);
    while (it.hasNext()) {
        it.next();
        qDebug() << "ChatID:" << it.key() << "AdminLogin:" << it.value();
    }
}

bool ChatItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        QPoint clickPos = mouseEvent->pos() - option.rect.topLeft();

        qDebug() << "Click Position relative to item:" << clickPos;

        QSize gearSize(24, 24);
        int gearX = option.rect.width() - gearSize.width() - 10;
        int gearY = (option.rect.height() - gearSize.height()) / 2;
        QRect gearRect(QPoint(gearX, gearY), gearSize);

        qDebug() << "Gear Rect:" << gearRect;

        QString chatID = index.data(Qt::UserRole).toString();
        bool isAdmin = adminChats.contains(chatID);
        qDebug() << "Is admin chat:" << isAdmin;

        if (gearRect.contains(clickPos) && isAdmin) {
            qDebug() << "Иконка шестерёнки нажата для чата:" << index.data(Qt::DisplayRole).toString();

            emit gearIconClicked(index);

            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ChatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QStyledItemDelegate::paint(painter, opt, index);

    painter->save();

    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QRect iconRect = option.rect.adjusted(5, 5, -5, -5);
    QSize iconSize(50, 50);
    QRect iconDrawRect(iconRect.topLeft(), iconSize);
    icon.paint(painter, iconDrawRect);

    QString chatName = index.data(Qt::DisplayRole).toString();
    QRect textRect = option.rect.adjusted(60, 10, -60, -10);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, chatName);

    QString chatID = index.data(Qt::UserRole).toString();
    if (adminChats.contains(chatID)) {
        if (!gearIcon.isNull()) {
            QSize gearSize(24, 24);
            int gearX = option.rect.width() - gearSize.width() - 10;
            int gearY = (option.rect.height() - gearSize.height()) / 2;
            QRect gearRect(QPoint(option.rect.left() + gearX, option.rect.top() + gearY), gearSize);

            gearIcon.paint(painter, gearRect, Qt::AlignCenter, QIcon::Normal, QIcon::Off);
        }
    }

    painter->restore();
}
