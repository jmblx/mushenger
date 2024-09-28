#ifndef CHATSCREEN_H
#define CHATSCREEN_H

#include <QObject>

class ChatScreen : public QObject
{
    Q_OBJECT
public:
    explicit ChatScreen(QObject *parent = nullptr);

signals:
};

#endif // CHATSCREEN_H
