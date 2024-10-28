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

    overlayLabel = nullptr;

    blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(0.0);
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
    animation->setEasingCurve(QEasingCurve::OutQuad);
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
    }

    QPixmap pixmap(iconPath);
    int iconWidth = pixmap.width() / 2 + 10;
    int iconHeight = pixmap.height() / 2 + 10;
    overlayLabel->setPixmap(pixmap.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    overlayLabel->setScaledContents(false);
}

void AnimatedButton::updateOverlayIcon(const QString &iconPath)
{
    if (overlayLabel) {
        overlayLabel->setPixmap(QPixmap(iconPath));
    }
}

void AnimatedButton::setHoverBlurEffect(QColor color, qreal maxBlurRadius, int duration)
{
    if (!blurEffect) {
        blurEffect = new QGraphicsBlurEffect(this);
        this->setGraphicsEffect(blurEffect);
    }

    QPropertyAnimation *blurAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
    blurAnimation->setDuration(duration);
    blurAnimation->setStartValue(0.0);
    blurAnimation->setEndValue(maxBlurRadius);
    blurAnimation->setEasingCurve(QEasingCurve::OutQuad);

    hoverEnterAnimation->addAnimation(blurAnimation);

    QPropertyAnimation *blurLeaveAnimation = new QPropertyAnimation(blurEffect, "blurRadius");
    blurLeaveAnimation->setDuration(duration);
    blurLeaveAnimation->setStartValue(maxBlurRadius);
    blurLeaveAnimation->setEndValue(0.0);
    blurLeaveAnimation->setEasingCurve(QEasingCurve::InQuad);

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
