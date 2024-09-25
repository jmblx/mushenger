#ifndef PROFILESCREEN_H
#define PROFILESCREEN_H

#include <QWidget>

namespace Ui {
    class ProfileScreen;
}

class ProfileScreen : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileScreen(QWidget *parent = nullptr);
    ~ProfileScreen();

private:
    Ui::ProfileScreen *ui;

signals:
};

#endif // PROFILESCREEN_H
