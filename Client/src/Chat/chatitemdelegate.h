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

    void setAdminChats(const QMap<QString, QString> &adminChatsMap);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

public slots:
    void onThemeChanged(const QString& newTheme);

signals:
    void gearIconClicked(const QModelIndex &index);

private:
    QMap<QString, QString> adminChats;
    QIcon gearIcon;
};

#endif
