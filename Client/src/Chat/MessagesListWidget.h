#ifndef MESSAGESLISTWIDGET_H
#define MESSAGESLISTWIDGET_H

#include <QListWidget>
#include "ui_ChatScreen.h"

class MessagesListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MessagesListWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateMessageWidgets();
};

#endif // MESSAGESLISTWIDGET_H
