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

    // Обновляем представление, чтобы перерисовать элементы с новой иконкой
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
    // Проверяем тип события
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        // Получаем позицию клика относительно элемента
        QPoint clickPos = mouseEvent->pos() - option.rect.topLeft(); // Координаты относительно текущего элемента

        qDebug() << "Click Position relative to item:" << clickPos;

        // Определяем область для иконки шестерёнки
        QSize gearSize(24, 24); // Размер иконки шестерёнки
        int gearX = option.rect.width() - gearSize.width() - 10; // Отступ от правого края элемента
        int gearY = (option.rect.height() - gearSize.height()) / 2; // Центр по вертикали элемента
        QRect gearRect(QPoint(gearX, gearY), gearSize);

        qDebug() << "Gear Rect:" << gearRect;

        // Проверяем, был ли клик в области шестерёнки и является ли пользователь администратором
        QString chatID = index.data(Qt::UserRole).toString();
        bool isAdmin = adminChats.contains(chatID);
        qDebug() << "Is admin chat:" << isAdmin;

        if (gearRect.contains(clickPos) && isAdmin) {
            qDebug() << "Иконка шестерёнки нажата для чата:" << index.data(Qt::DisplayRole).toString();

            // Вызываем сигнал, связанный с действием (например, для открытия меню)
            emit gearIconClicked(index);

            return true; // Обрабатываем событие, чтобы не передавать его дальше
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ChatItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    painter->save();

    // Отрисовка фона элемента
    if (opt.state & QStyle::State_Selected) {
        painter->fillRect(opt.rect, opt.palette.highlight());
    } else {
        painter->fillRect(opt.rect, opt.palette.base());
    }

    // Отрисовка аватарки
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    QRect iconRect = option.rect.adjusted(5, 5, -5, -5); // Оставляем небольшой отступ
    QSize iconSize(50, 50); // Размер аватарки
    QRect iconDrawRect(iconRect.topLeft(), iconSize);
    icon.paint(painter, iconDrawRect);

    // Отрисовка названия чата
    QString chatName = index.data(Qt::DisplayRole).toString();
    QRect textRect = option.rect.adjusted(60, 10, -60, -10); // Увеличение правого отступа для предотвращения обрезки
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, chatName);

    // Отрисовка шестерёнки только для администраторов
    QString chatID = index.data(Qt::UserRole).toString();
    if (adminChats.contains(chatID)) {
        // Отрисовка иконки шестерёнки
        if (!gearIcon.isNull()) {
            QSize gearSize(24, 24); // Размер иконки шестерёнки
            int gearX = option.rect.width() - gearSize.width() - 10; // Отступ от правого края элемента
            int gearY = (option.rect.height() - gearSize.height()) / 2; // Центр по вертикали элемента
            QRect gearRect(QPoint(option.rect.left() + gearX, option.rect.top() + gearY), gearSize);

            // Рисуем шестерёнку
            gearIcon.paint(painter, gearRect, Qt::AlignCenter, QIcon::Normal, QIcon::Off);
        }
    }

    painter->restore();
}
