/********************************************************************************
** Form generated from reading UI file 'ProfileScreen.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILESCREEN_H
#define UI_PROFILESCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/AnimatedButton/AnimatedButton.h"
#include "src/AnimatedComboBox/AnimatedComboBox.h"
#include "src/AnimatedSpinBox/AnimatedSpinBox.h"

QT_BEGIN_NAMESPACE

class Ui_ProfileScreen
{
public:
    QFrame *profilePanel;
    QLabel *profileIcon;
    QLabel *loginLabel;
    QWidget *logoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *logoutButtonIcon;
    QLabel *logoutText;
    AnimatedButton *overlayButton;
    AnimatedButton *profileButton;
    QLabel *fontSizeLabel;
    AnimatedSpinBox *fontSizeSpinBox;
    QLabel *fontLabel;
    AnimatedComboBox *fontComboBox;
    AnimatedButton *backButton;
    QLabel *mushroomMessage;
    QPushButton *themeSwitchButton;
    QLabel *background;

    void setupUi(QWidget *ProfileScreen)
    {
        if (ProfileScreen->objectName().isEmpty())
            ProfileScreen->setObjectName("ProfileScreen");
        ProfileScreen->resize(1440, 1024);
        ProfileScreen->setMinimumSize(QSize(1440, 1024));
        ProfileScreen->setMaximumSize(QSize(1440, 1024));
        profilePanel = new QFrame(ProfileScreen);
        profilePanel->setObjectName("profilePanel");
        profilePanel->setGeometry(QRect(200, 50, 408, 342));
        profilePanel->setFrameShape(QFrame::Shape::NoFrame);
        profilePanel->setFrameShadow(QFrame::Shadow::Plain);
        profileIcon = new QLabel(profilePanel);
        profileIcon->setObjectName("profileIcon");
        profileIcon->setGeometry(QRect(130, 40, 161, 161));
        profileIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/light/profile-circled.svg")));
        profileIcon->setScaledContents(true);
        loginLabel = new QLabel(profilePanel);
        loginLabel->setObjectName("loginLabel");
        loginLabel->setGeometry(QRect(54, 210, 300, 40));
        loginLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginLabel->setWordWrap(true);
        logoutWidget = new QWidget(profilePanel);
        logoutWidget->setObjectName("logoutWidget");
        logoutWidget->setGeometry(QRect(350, 20, 46, 61));
        verticalLayout = new QVBoxLayout(logoutWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        logoutButtonIcon = new QPushButton(logoutWidget);
        logoutButtonIcon->setObjectName("logoutButtonIcon");
        logoutButtonIcon->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":images/light/door.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        logoutButtonIcon->setIcon(icon);
        logoutButtonIcon->setIconSize(QSize(32, 32));
        logoutButtonIcon->setFlat(true);

        verticalLayout->addWidget(logoutButtonIcon);

        logoutText = new QLabel(logoutWidget);
        logoutText->setObjectName("logoutText");
        logoutText->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(logoutText);

        overlayButton = new AnimatedButton(profilePanel);
        overlayButton->setObjectName("overlayButton");
        overlayButton->setGeometry(QRect(350, 20, 40, 61));
        overlayButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        overlayButton->setFlat(true);
        profileButton = new AnimatedButton(profilePanel);
        profileButton->setObjectName("profileButton");
        profileButton->setGeometry(QRect(130, 40, 161, 161));
        profileButton->setFlat(true);
        fontSizeLabel = new QLabel(ProfileScreen);
        fontSizeLabel->setObjectName("fontSizeLabel");
        fontSizeLabel->setGeometry(QRect(770, 80, 181, 111));
        fontSizeLabel->setWordWrap(true);
        fontSizeSpinBox = new AnimatedSpinBox(ProfileScreen);
        fontSizeSpinBox->setObjectName("fontSizeSpinBox");
        fontSizeSpinBox->setGeometry(QRect(1000, 80, 263, 105));
        fontSizeSpinBox->setProperty("minimum", QVariant(10));
        fontSizeSpinBox->setProperty("maximum", QVariant(200));
        fontSizeSpinBox->setProperty("value", QVariant(128));
        fontLabel = new QLabel(ProfileScreen);
        fontLabel->setObjectName("fontLabel");
        fontLabel->setGeometry(QRect(770, 240, 171, 105));
        fontComboBox = new AnimatedComboBox(ProfileScreen);
        fontComboBox->setObjectName("fontComboBox");
        fontComboBox->setGeometry(QRect(1000, 220, 261, 105));
        backButton = new AnimatedButton(ProfileScreen);
        backButton->setObjectName("backButton");
        backButton->setGeometry(QRect(10, 10, 28, 28));
        backButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":images/light/out_pointer.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        backButton->setIcon(icon1);
        backButton->setIconSize(QSize(22, 22));
        backButton->setFlat(true);
        mushroomMessage = new QLabel(ProfileScreen);
        mushroomMessage->setObjectName("mushroomMessage");
        mushroomMessage->setGeometry(QRect(700, 650, 339, 160));
        mushroomMessage->setAlignment(Qt::AlignmentFlag::AlignCenter);
        mushroomMessage->setWordWrap(true);
        themeSwitchButton = new QPushButton(ProfileScreen);
        themeSwitchButton->setObjectName("themeSwitchButton");
        themeSwitchButton->setGeometry(QRect(1330, 0, 111, 101));
        themeSwitchButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        background = new QLabel(ProfileScreen);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 1440, 1024));
        background->setPixmap(QPixmap(QString::fromUtf8(":/images/light/profile_screen.png")));
        background->setScaledContents(true);
        background->raise();
        backButton->raise();
        profilePanel->raise();
        fontSizeLabel->raise();
        fontSizeSpinBox->raise();
        fontLabel->raise();
        fontComboBox->raise();
        mushroomMessage->raise();
        themeSwitchButton->raise();

        retranslateUi(ProfileScreen);

        QMetaObject::connectSlotsByName(ProfileScreen);
    } // setupUi

    void retranslateUi(QWidget *ProfileScreen)
    {
        ProfileScreen->setWindowTitle(QCoreApplication::translate("ProfileScreen", "Profile Screen", nullptr));
        loginLabel->setText(QCoreApplication::translate("ProfileScreen", "{username}", nullptr));
        logoutButtonIcon->setText(QString());
        logoutText->setText(QCoreApplication::translate("ProfileScreen", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        overlayButton->setText(QString());
        profileButton->setText(QString());
        fontSizeLabel->setText(QCoreApplication::translate("ProfileScreen", "\320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260:", nullptr));
        fontLabel->setText(QCoreApplication::translate("ProfileScreen", "\320\250\321\200\320\270\321\204\321\202:", nullptr));
        backButton->setText(QString());
        mushroomMessage->setText(QCoreApplication::translate("ProfileScreen", "{message}", nullptr));
        themeSwitchButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ProfileScreen: public Ui_ProfileScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILESCREEN_H
