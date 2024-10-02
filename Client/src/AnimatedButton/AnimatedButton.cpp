#include "AnimatedButton.h"
#include <QEvent>
#include <QEnterEvent>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>
#include <QDebug>

AnimatedButton::AnimatedButton(QWidget *parent) : QPushButton(parent)
{
    hoverEnterAnimation = new QParallelAnimationGroup(this);
    hoverLeaveAnimation = new QParallelAnimationGroup(this);
    clickAnimation = new QParallelAnimationGroup(this);
    releaseAnimation = new QParallelAnimationGroup(this);

    overlayLabel = nullptr; // Инициализируем overlayLabel

    // Инициализируем colorizeEffect для затемнения
    colorizeEffect = new QGraphicsColorizeEffect(this);
    colorizeEffect->setColor(Qt::black); // Цвет затемнения
    colorizeEffect->setStrength(0.0);    // Начальная сила эффекта
    this->setGraphicsEffect(colorizeEffect);
}

void AnimatedButton::setHoverAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration)
{
    addAnimation(hoverEnterAnimation, this, propertyName, startValue, endValue, duration);
    addAnimation(hoverLeaveAnimation, this, propertyName, endValue, startValue, duration);
}

void AnimatedButton::setClickAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration)
{
    addAnimation(clickAnimation, this, propertyName, startValue, endValue, duration);
    addAnimation(releaseAnimation, this, propertyName, endValue, startValue, duration);
}

void AnimatedButton::addAnimation(QParallelAnimationGroup *group, QObject *target, const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(target, propertyName);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    group->addAnimation(animation);
}

void AnimatedButton::setupOverlayLabel(const QString &iconPath)
{
    if (!overlayLabel) {
        overlayLabel = new QLabel(this);
        overlayLabel->setAlignment(Qt::AlignCenter);
        overlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        overlayLabel->setGeometry(this->rect());
        overlayLabel->hide();

        // Добавляем QGraphicsOpacityEffect
        overlayOpacityEffect = new QGraphicsOpacityEffect(overlayLabel);
        overlayOpacityEffect->setOpacity(0.0);
        overlayLabel->setGraphicsEffect(overlayOpacityEffect);
    }

    overlayLabel->setPixmap(QPixmap(iconPath));
    overlayLabel->setScaledContents(true);

    // Анимация для появления overlayLabel
    addAnimation(hoverEnterAnimation, overlayOpacityEffect, "opacity", 0.0, 1.0, 200);

    // Анимация для исчезновения overlayLabel
    addAnimation(hoverLeaveAnimation, overlayOpacityEffect, "opacity", 1.0, 0.0, 200);
}

void AnimatedButton::updateOverlayIcon(const QString &iconPath)
{
    if (overlayLabel) {
        overlayLabel->setPixmap(QPixmap(iconPath));
    }
}

void AnimatedButton::setHoverDarkenEffect(qreal strength, int duration)
{
    // Анимация при наведении
    addAnimation(hoverEnterAnimation, colorizeEffect, "strength", 0.0, strength, duration);

    // Анимация при уходе курсора
    addAnimation(hoverLeaveAnimation, colorizeEffect, "strength", strength, 0.0, duration);
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

void AnimatedButton::mousePressEvent(QMouseEvent *event)
{
    clickAnimation->start();
    QPushButton::mousePressEvent(event);
}

void AnimatedButton::mouseReleaseEvent(QMouseEvent *event)
{
    releaseAnimation->start();
    QPushButton::mouseReleaseEvent(event);
}
