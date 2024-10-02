#ifndef ANIMATEDBUTTON_H
#define ANIMATEDBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QEnterEvent>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>

class AnimatedButton : public QPushButton
{
    Q_OBJECT

public:
    explicit AnimatedButton(QWidget *parent = nullptr);

    // Методы для настройки анимации
    void setHoverAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration = 200);
    void setClickAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration = 100);

    // Методы для управления overlay
    void setupOverlayLabel(const QString &iconPath);
    void updateOverlayIcon(const QString &iconPath);

    // Метод для установки эффекта затемнения при наведении
    void setHoverDarkenEffect(qreal strength = 0.3, int duration = 200);

signals:
    void hoverEntered();
    void hoverLeft();

protected:
    // Переопределение событий
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Анимационные группы для наведения и нажатия
    QParallelAnimationGroup *hoverEnterAnimation;
    QParallelAnimationGroup *hoverLeaveAnimation;
    QParallelAnimationGroup *clickAnimation;
    QParallelAnimationGroup *releaseAnimation;

    QLabel *overlayLabel;
    QGraphicsOpacityEffect *overlayOpacityEffect;
    QGraphicsColorizeEffect *colorizeEffect;

    // Метод для создания анимации
    void addAnimation(QParallelAnimationGroup *group, QObject *target, const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration);

    // Флаги состояния
    bool isHovered = false;
};

#endif // ANIMATEDBUTTON_H
