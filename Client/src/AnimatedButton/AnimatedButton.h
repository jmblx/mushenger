#ifndef ANIMATEDBUTTON_H
#define ANIMATEDBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QEnterEvent>
#include <QGraphicsBlurEffect>

class AnimatedButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

public:
    explicit AnimatedButton(QWidget *parent = nullptr);

    void setHoverAnimationProperty(const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration = 200);

    void setupOverlayLabel(const QString &iconPath);
    void updateOverlayIcon(const QString &iconPath);

    void setHoverBlurEffect(QColor color, qreal maxBlurRadius = 10.0, int duration = 200);

    QParallelAnimationGroup* getHoverEnterAnimation() const { return hoverEnterAnimation; }
    QParallelAnimationGroup* getHoverLeaveAnimation() const { return hoverLeaveAnimation; }

    QSize iconSize() const;
    void setIconSize(const QSize &size);

signals:
    void hoverEntered();
    void hoverLeft();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QParallelAnimationGroup *hoverEnterAnimation;
    QParallelAnimationGroup *hoverLeaveAnimation;
    QLabel *overlayLabel;
    QGraphicsBlurEffect *blurEffect;

    void addAnimation(QParallelAnimationGroup *group, QObject *target, const QByteArray &propertyName, const QVariant &startValue, const QVariant &endValue, int duration);

    bool isHovered = false;
};

#endif // ANIMATEDBUTTON_H
