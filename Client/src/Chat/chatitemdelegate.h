#ifndef CHATITEMDELEGATE_H
#define CHATITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>
#include <QModelIndex>
#include <QWidget>
#include <QPainter>
#include <QStyleOptionViewItem>

class ChatItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ChatItemDelegate(QObject *parent = nullptr);

    // Метод для установки списка чатов, где текущий пользователь является админом
    void setAdminChats(const QMap<QString, QString> &adminChatsMap);

    // Переопределяем метод для рисования
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // Переопределяем метод для обработки событий
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

public slots:
    void onThemeChanged(const QString& newTheme);

signals:
    // Сигнал для оповещения о клике по шестерёнке
    void gearIconClicked(const QModelIndex &index);

private:
    // Список чатов, где текущий пользователь является админом
    QMap<QString, QString> adminChats;
    QIcon gearIcon; // Иконка шестерёнки
};

#endif
