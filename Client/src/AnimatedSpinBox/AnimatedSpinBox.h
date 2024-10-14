#ifndef ANIMATEDSPINBOX_H
#define ANIMATEDSPINBOX_H

#include <QSpinBox>
#include "src/AnimatedButton/AnimatedButton.h"

class AnimatedSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit AnimatedSpinBox(QWidget *parent = nullptr);
    ~AnimatedSpinBox();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    AnimatedButton* upButton;
    AnimatedButton* downButton;

    void setupButtons();
};

#endif // ANIMATEDSPINBOX_H
