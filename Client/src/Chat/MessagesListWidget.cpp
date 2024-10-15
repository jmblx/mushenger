#include "MessagesListWidget.h"
#include <QResizeEvent>
#include <QListWidgetItem>
#include <QDebug>

MessagesListWidget::MessagesListWidget(QWidget *parent)
    : QListWidget(parent)
{
    // Настройка QListWidget
    setFlow(QListView::TopToBottom);
    setWrapping(false);
    setResizeMode(QListView::Adjust);
    setVerticalScrollMode(QListView::ScrollPerPixel);
}

void MessagesListWidget::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);
    updateMessageWidgets();
}

void MessagesListWidget::updateMessageWidgets()
{
    int maxWidth = width() * 0.7; // 70% ширины списка сообщений

    for(int i = 0; i < count(); ++i){
        QListWidgetItem *item = this->item(i);
        QWidget *widget = this->itemWidget(item);
        if(widget){
            widget->setMaximumWidth(maxWidth);
            // Обновляем размер элемента списка
            item->setSizeHint(widget->sizeHint());
        }
    }
}
