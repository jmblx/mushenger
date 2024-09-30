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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QPushButton *overlayButton;
    QLabel *mushroomMessage;
    QLabel *fontSizeLabel;
    QSpinBox *fontSizeSpinBox;
    QLabel *fontLabel;
    QComboBox *fontComboBox;
    QPushButton *backButton;
    QLabel *background;
    QPushButton *overlayButton_arrow;

    void setupUi(QWidget *ProfileScreen)
    {
        if (ProfileScreen->objectName().isEmpty())
            ProfileScreen->setObjectName("ProfileScreen");
        ProfileScreen->resize(1440, 1024);
        ProfileScreen->setMinimumSize(QSize(1440, 1024));
        ProfileScreen->setMaximumSize(QSize(1440, 1024));
        ProfileScreen->setAutoFillBackground(false);
        ProfileScreen->setStyleSheet(QString::fromUtf8("\n"
"    QFrame#profilePanel {\n"
"      border: 2px solid black;\n"
"      border-radius: 10px;\n"
"      background-color: transparent;\n"
"    }\n"
"    * {\n"
"      font-family: 'Inter';\n"
"    }\n"
"   "));
        profilePanel = new QFrame(ProfileScreen);
        profilePanel->setObjectName("profilePanel");
        profilePanel->setGeometry(QRect(200, 50, 408, 342));
        profilePanel->setFrameShape(QFrame::Shape::NoFrame);
        profilePanel->setFrameShadow(QFrame::Shadow::Plain);
        profileIcon = new QLabel(profilePanel);
        profileIcon->setObjectName("profileIcon");
        profileIcon->setGeometry(QRect(130, 40, 161, 151));
        profileIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/profile-circled.svg")));
        profileIcon->setScaledContents(true);
        loginLabel = new QLabel(profilePanel);
        loginLabel->setObjectName("loginLabel");
        loginLabel->setGeometry(QRect(54, 180, 300, 131));
        loginLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        loginLabel->setWordWrap(true);
        logoutWidget = new QWidget(profilePanel);
        logoutWidget->setObjectName("logoutWidget");
        logoutWidget->setGeometry(QRect(350, 20, 40, 61));
        verticalLayout = new QVBoxLayout(logoutWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        logoutButtonIcon = new QPushButton(logoutWidget);
        logoutButtonIcon->setObjectName("logoutButtonIcon");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":images/door.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        logoutButtonIcon->setIcon(icon);
        logoutButtonIcon->setIconSize(QSize(32, 32));

        verticalLayout->addWidget(logoutButtonIcon);

        logoutText = new QLabel(logoutWidget);
        logoutText->setObjectName("logoutText");
        logoutText->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(logoutText);

        overlayButton = new QPushButton(profilePanel);
        overlayButton->setObjectName("overlayButton");
        overlayButton->setGeometry(QRect(350, 20, 40, 61));
        overlayButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        mushroomMessage = new QLabel(ProfileScreen);
        mushroomMessage->setObjectName("mushroomMessage");
        mushroomMessage->setGeometry(QRect(700, 650, 339, 160));
        mushroomMessage->setAlignment(Qt::AlignmentFlag::AlignCenter);
        mushroomMessage->setWordWrap(true);
        fontSizeLabel = new QLabel(ProfileScreen);
        fontSizeLabel->setObjectName("fontSizeLabel");
        fontSizeLabel->setGeometry(QRect(770, 80, 181, 111));
        fontSizeLabel->setStyleSheet(QString::fromUtf8("color: #0D1321; font-size: 46px;"));
        fontSizeLabel->setWordWrap(true);
        fontSizeSpinBox = new QSpinBox(ProfileScreen);
        fontSizeSpinBox->setObjectName("fontSizeSpinBox");
        fontSizeSpinBox->setGeometry(QRect(1000, 80, 263, 105));
        fontSizeSpinBox->setMinimum(10);
        fontSizeSpinBox->setMaximum(200);
        fontSizeSpinBox->setValue(128);
        fontLabel = new QLabel(ProfileScreen);
        fontLabel->setObjectName("fontLabel");
        fontLabel->setGeometry(QRect(770, 240, 171, 105));
        fontComboBox = new QComboBox(ProfileScreen);
        fontComboBox->addItem(QString());
        fontComboBox->setObjectName("fontComboBox");
        fontComboBox->setGeometry(QRect(1000, 220, 261, 105));
        backButton = new QPushButton(ProfileScreen);
        backButton->setObjectName("backButton");
        backButton->setGeometry(QRect(10, 10, 28, 28));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":images/out_pointer.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        backButton->setIcon(icon1);
        backButton->setIconSize(QSize(22, 22));
        background = new QLabel(ProfileScreen);
        background->setObjectName("background");
        background->setGeometry(QRect(-2, -5, 1451, 1031));
        background->setPixmap(QPixmap(QString::fromUtf8(":/images/profile_screen.png")));
        background->setScaledContents(true);
        overlayButton_arrow = new QPushButton(ProfileScreen);
        overlayButton_arrow->setObjectName("overlayButton_arrow");
        overlayButton_arrow->setGeometry(QRect(10, 10, 28, 28));
        overlayButton_arrow->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        background->raise();
        backButton->raise();
        fontSizeLabel->raise();
        fontSizeSpinBox->raise();
        fontLabel->raise();
        fontComboBox->raise();
        profilePanel->raise();
        mushroomMessage->raise();
        overlayButton_arrow->raise();

        retranslateUi(ProfileScreen);

        QMetaObject::connectSlotsByName(ProfileScreen);
    } // setupUi

    void retranslateUi(QWidget *ProfileScreen)
    {
        ProfileScreen->setWindowTitle(QCoreApplication::translate("ProfileScreen", "Profile Screen", nullptr));
        loginLabel->setStyleSheet(QCoreApplication::translate("ProfileScreen", "color: #0D1321; font-size: 48px;", nullptr));
        loginLabel->setText(QCoreApplication::translate("ProfileScreen", "{username}", nullptr));
        logoutButtonIcon->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"         QPushButton {\n"
"          border: none;\n"
"          background: none;\n"
"          padding: 0px;\n"
"         }\n"
"        ", nullptr));
        logoutText->setStyleSheet(QCoreApplication::translate("ProfileScreen", "color: #0D1321; font-size: 12px;", nullptr));
        logoutText->setText(QCoreApplication::translate("ProfileScreen", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        overlayButton->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"      QPushButton {\n"
"       background: transparent;\n"
"       border: none;\n"
"      }\n"
"     ", nullptr));
        overlayButton->setText(QString());
        mushroomMessage->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"     QLabel {\n"
"      background-color: #D8DCE4;\n"
"      border-radius: 10px;\n"
"      color: #0D1321;\n"
"      font-size: 33px;\n"
"     }\n"
"    ", nullptr));
        mushroomMessage->setText(QCoreApplication::translate("ProfileScreen", "{message}", nullptr));
        fontSizeLabel->setText(QCoreApplication::translate("ProfileScreen", "\320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260:", nullptr));
        fontSizeSpinBox->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"     QSpinBox {\n"
"      color: #0D1321;\n"
"      background-color: #E5E5E5;\n"
"      font-size: 48px;\n"
"      border: none;\n"
"      padding-right: 90px;\n"
"      padding-left: 10px;\n"
"     }\n"
"     QSpinBox::up-button {\n"
"      subcontrol-origin: padding;\n"
"      subcontrol-position: right;\n"
"      width: 50px;\n"
"      height: 105px;\n"
"      background-color: #FFFFFF;\n"
"      border: none;\n"
"     }\n"
"     QSpinBox::down-button {\n"
"      subcontrol-origin: padding;\n"
"      subcontrol-position: right;\n"
"      right: 55px;\n"
"      width: 50px;\n"
"      height: 105px;\n"
"      background-color: #FFFFFF;\n"
"      border: none;\n"
"     }\n"
"     QSpinBox::up-arrow {\n"
"      image: url(:/images/plus_icon.svg);\n"
"      width: 20px;\n"
"      height: 20px;\n"
"     }\n"
"     QSpinBox::down-arrow {\n"
"      image: url(:/images/minus_icon.svg);\n"
"      width: 20px;\n"
"      height: 20px;\n"
"     }\n"
"    ", nullptr));
        fontLabel->setStyleSheet(QCoreApplication::translate("ProfileScreen", "color: #0D1321; font-size: 48px;", nullptr));
        fontLabel->setText(QCoreApplication::translate("ProfileScreen", "\320\250\321\200\320\270\321\204\321\202:", nullptr));
        fontComboBox->setItemText(0, QCoreApplication::translate("ProfileScreen", "Inter", nullptr));

        fontComboBox->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"     QComboBox {\n"
"      color: #0D1321;\n"
"      background-color: #E5E5E5;\n"
"      font-size: 48px;\n"
"      padding-left: 10px;\n"
"      padding-right: 50px;\n"
"      border: none;\n"
"     }\n"
"     QComboBox::drop-down {\n"
"      subcontrol-origin: padding;\n"
"      subcontrol-position: right;\n"
"      width: 50px;\n"
"      background-color: #FFFFFF;\n"
"      border: none;\n"
"     }\n"
"     QComboBox::down-arrow {\n"
"      image: url(:/images/down_arrow_icon.svg);\n"
"      width: 20px;\n"
"      height: 20px;\n"
"     }\n"
"    QComboBox QAbstractItemView {\n"
"      color: #0D1321;\n"
"      background-color: #E5E5E5;\n"
"      font-size: 48px;\n"
"      selection-background-color: #D3D3D3;\n"
"     }\n"
"    ", nullptr));
        backButton->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"     QPushButton {\n"
"      border: none;\n"
"      background: none;\n"
"     }\n"
"     QPushButton::hover {\n"
"      background: none;\n"
"     }\n"
"    ", nullptr));
        background->setText(QString());
        overlayButton_arrow->setStyleSheet(QCoreApplication::translate("ProfileScreen", "\n"
"      QPushButton {\n"
"       background: transparent;\n"
"       border: none;\n"
"      }\n"
"     ", nullptr));
        overlayButton_arrow->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ProfileScreen: public Ui_ProfileScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILESCREEN_H
