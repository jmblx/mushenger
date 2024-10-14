#include "AnimatedButton.h"
#include <QEvent>
#include <QEnterEvent>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include <QEasingCurve>

AnimatedButton::AnimatedButton(QWidget *parent) : QPushButton(parent)
{
    hoverEnterAnimation = new QParallelAnimationGroup(this);
    hoverLeaveAnimation = new QParallelAnimationGroup(this);

    overlayLabel = nullptr; // Инициализируем overlayLabel

    blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(0.0); // Начальная интенсивность размытия
    this->setGraphicsEffect(blurEffect);
}

QSize AnimatedButton::iconSize() const
{
    return QPushButton::iconSize();
}

void AnimatedButton::setIconSize(const QSize &size)
{
    QPushButton::setIconSize(size);
    update();
}

void AnimatedButton::setHoverAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration)
{
    addAnimation(hoverEnterAnimation, this, propertyName, startValue, endValue, duration);
    addAnimation(hoverLeaveAnimation, this, propertyName, endValue, startValue, duration);
}

void AnimatedButton::addAnimation(QParallelAnimationGroup *group, QObject *target, const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(target, propertyName);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutQuad); // Добавление кривой сглаживания
    group->addAnimation(animation);
}

void AnimatedButton::setupOverlayLabel(const QString &iconPath)
{
    if (!overlayLabel) {
        overlayLabel = new QLabel(this);
        overlayLabel->setAlignment(Qt::AlignCenter);  // Центрирование иконки внутри QLabel
        overlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        overlayLabel->setGeometry(this->rect());  // QLabel занимает всю кнопку
        overlayLabel->hide();  // Скрываем иконку до наведения
    }

    // Задаем иконку и уменьшаем ее размеры
    QPixmap pixmap(iconPath);
    int iconWidth = pixmap.width() / 2 + 10;   // Уменьшаем иконку до 50% от оригинала
    int iconHeight = pixmap.height() / 2 + 10; // Уменьшаем иконку до 50% от оригинала
    overlayLabel->setPixmap(pixmap.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    overlayLabel->setScaledContents(false);  // Отключаем автоматическое масштабирование, чтобы сохранять пропорции
}

void AnimatedButton::updateOverlayIcon(const QString &iconPath)
{
    if (overlayLabel) {
        overlayLabel->setPixmap(QPixmap(iconPath));
    }
}

void AnimatedButton::setHoverBlurEffect(QColor color, qreal maxBlurRadius, int duration)
{
    // Убедитесь, что эффект размытия применяется к overlayLabel, если он существует
    if (!blurEffect) {
        blurEffect = new QGraphicsBlurEffect(this);
        this->setGraphicsEffect(blurEffect);  // Применяем эффект к кнопке
    }

    // Создаём анимацию для свойства blurRadius у blurEffect
    QPropertyAnimation *blurAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
    blurAnimation->setDuration(duration);
    blurAnimation->setStartValue(0.0);  // Начальное значение размытия
    blurAnimation->setEndValue(maxBlurRadius);  // Конечное значение размытия
    blurAnimation->setEasingCurve(QEasingCurve::OutQuad); // Добавление кривой сглаживания

    // Добавляем анимацию в группу для входа курсора
    hoverEnterAnimation->addAnimation(blurAnimation);

    // Анимация для ухода курсора
    QPropertyAnimation *blurLeaveAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
    blurLeaveAnimation->setDuration(duration);
    blurLeaveAnimation->setStartValue(maxBlurRadius);
    blurLeaveAnimation->setEndValue(0.0);
    blurLeaveAnimation->setEasingCurve(QEasingCurve::InQuad); // Добавление кривой сглаживания

    hoverLeaveAnimation->addAnimation(blurLeaveAnimation);
}

void AnimatedButton::enterEvent(QEnterEvent *event)
{
    if (overlayLabel) overlayLabel->show();

    hoverLeaveAnimation->stop();
    hoverEnterAnimation->stop();
    hoverEnterAnimation->start();

    emit hoverEntered();

    QPushButton::enterEvent(event);
}

void AnimatedButton::leaveEvent(QEvent *event)
{
    hoverEnterAnimation->stop();
    hoverLeaveAnimation->stop();
    hoverLeaveAnimation->start();

    emit hoverLeft();

    if (overlayLabel) overlayLabel->hide();

    QPushButton::leaveEvent(event);
}
