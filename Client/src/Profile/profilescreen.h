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

private slots:
    void onOverlayButtonClicked();

private:
    Ui::ProfileScreen *ui;
};

#endif // PROFILESCREEN_H
