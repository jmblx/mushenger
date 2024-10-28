#include "AnimatedComboBox.h"
#include <QHBoxLayout>

AnimatedComboBox::AnimatedComboBox(QWidget *parent) : QComboBox(parent), dropDownButton(nullptr)
{
    setStyleSheet("AnimatedComboBox#fontComboBox::drop-down { width: 0px; border: none; background: transparent; }");
    setEditable(false);
    setupButton();
}

AnimatedComboBox::~AnimatedComboBox() {}

void AnimatedComboBox::setupButton()
{
    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(buttonContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    dropDownButton = new AnimatedButton(buttonContainer);
    dropDownButton->setIcon(QIcon(":/images/light/down_arrow_icon.svg"));
    dropDownButton->setIconSize(QSize(20, 20));
    dropDownButton->setFlat(true);
    dropDownButton->setCursor(Qt::PointingHandCursor);
    dropDownButton->setFixedSize(50, 105);
    dropDownButton->setStyleSheet("background-color: white; border: none;");

    dropDownButton->setHoverAnimationProperty("iconSize", QSize(20, 20), QSize(24, 24), 200);

    connect(dropDownButton, &QPushButton::clicked, this, &QComboBox::showPopup);

    layout->addWidget(dropDownButton);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonContainer);
}

void AnimatedComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
}
