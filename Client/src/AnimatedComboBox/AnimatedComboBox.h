#ifndef ANIMATEDCOMBOBOX_H
#define ANIMATEDCOMBOBOX_H

#include <QComboBox>
#include "src/AnimatedButton/AnimatedButton.h"

class AnimatedComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AnimatedComboBox(QWidget *parent = nullptr);
    ~AnimatedComboBox();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    AnimatedButton* dropDownButton;

    void setupButton();
};

#endif // ANIMATEDCOMBOBOX_H
