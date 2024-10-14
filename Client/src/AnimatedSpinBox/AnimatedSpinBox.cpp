#include "AnimatedSpinBox.h"
#include <QStyle>
#include <QHBoxLayout>

AnimatedSpinBox::AnimatedSpinBox(QWidget *parent) : QSpinBox(parent), upButton(nullptr), downButton(nullptr)
{
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    setStyleSheet("QSpinBox { padding-right: 0px; }");

    setupButtons();
}

AnimatedSpinBox::~AnimatedSpinBox() {}

void AnimatedSpinBox::setupButtons()
{
    // Создаем контейнер для кнопок
    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(buttonContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Создаем кнопку "Вверх"
    upButton = new AnimatedButton(buttonContainer);
    upButton->setIcon(QIcon(":/images/light/plus_icon.svg"));
    upButton->setIconSize(QSize(20, 20));
    upButton->setFlat(true);
    upButton->setCursor(Qt::PointingHandCursor);
    upButton->setFixedSize(50, 105); // Устанавливаем размер
    upButton->setStyleSheet("background-color: white; border: none;");

    upButton->setHoverAnimationProperty("iconSize", QSize(20, 20), QSize(24, 24), 200);

    connect(upButton, &QPushButton::clicked, this, [=]() {
        stepUp();
    });

    // Добавляем кнопку "Вверх" в контейнер
    layout->addWidget(upButton);

    // Создаем кнопку "Вниз"
    downButton = new AnimatedButton(buttonContainer);
    downButton->setIcon(QIcon(":/images/light/minus_icon.svg"));
    downButton->setIconSize(QSize(20, 20));
    downButton->setFlat(true);
    downButton->setCursor(Qt::PointingHandCursor);
    downButton->setFixedSize(50, 105); // Устанавливаем размер
    downButton->setStyleSheet("background-color: white; border: none;"); // Устанавливаем белый фон

    // Применяем анимацию к кнопке "Вниз"
    downButton->setHoverAnimationProperty("iconSize", QSize(20, 20), QSize(24, 24), 200);

    // Соединяем сигнал нажатия с уменьшением значения
    connect(downButton, &QPushButton::clicked, this, [=]() {
        stepDown();
    });

    // Добавляем кнопку "Вниз" в контейнер
    layout->addWidget(downButton);

    // Размещаем контейнер кнопок поверх QSpinBox
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonContainer);
}

void AnimatedSpinBox::resizeEvent(QResizeEvent *event)
{
    QSpinBox::resizeEvent(event);
}
