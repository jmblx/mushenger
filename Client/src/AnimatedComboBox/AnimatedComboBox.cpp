#include "AnimatedComboBox.h"
#include <QHBoxLayout>

AnimatedComboBox::AnimatedComboBox(QWidget *parent) : QComboBox(parent), dropDownButton(nullptr)
{
    setStyleSheet("AnimatedComboBox#fontComboBox::drop-down { width: 0px; border: none; background: transparent; }");
    setEditable(false); // Сделать комбо-бокс не редактируемым, если это необходимо
    setupButton();
}

AnimatedComboBox::~AnimatedComboBox() {}

void AnimatedComboBox::setupButton()
{
    // Создаем контейнер для кнопки
    QWidget* buttonContainer = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(buttonContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Создаем кнопку раскрытия
    dropDownButton = new AnimatedButton(buttonContainer);
    dropDownButton->setIcon(QIcon(":/images/light/down_arrow_icon.svg"));
    dropDownButton->setIconSize(QSize(20, 20));
    dropDownButton->setFlat(true);
    dropDownButton->setCursor(Qt::PointingHandCursor);
    dropDownButton->setFixedSize(50, 105); // Устанавливаем размер
    dropDownButton->setStyleSheet("background-color: white; border: none;"); // Устанавливаем белый фон

    // Применяем анимацию к кнопке
    dropDownButton->setHoverAnimationProperty("iconSize", QSize(20, 20), QSize(24, 24), 200);

    // Соединяем сигнал нажатия с открытием списка
    connect(dropDownButton, &QPushButton::clicked, this, &QComboBox::showPopup);

    // Добавляем кнопку в контейнер
    layout->addWidget(dropDownButton);

    // Размещаем контейнер кнопки поверх QComboBox
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonContainer);
}

void AnimatedComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
    // Обновляем позиционирование контейнера кнопки при изменении размера, если нужно
}
