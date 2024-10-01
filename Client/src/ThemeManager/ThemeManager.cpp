#include "ThemeManager.h"
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QDebug>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager()
{
    loadThemeFromSettings();
}

void ThemeManager::loadThemeFromSettings()
{
    QSettings settings("IS32", "mushenger");
    m_currentTheme = settings.value("theme", "light").toString();
}

void ThemeManager::saveThemeToSettings()
{
    QSettings settings("IS32", "mushenger");
    settings.setValue("theme", m_currentTheme);
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::setTheme(const QString& theme)
{
    m_currentTheme = theme;
    saveThemeToSettings();

    // Загрузка и применение стилей
    QString stylePath = QString(":/styles/%1.qss").arg(m_currentTheme);
    QFile styleFile(stylePath);
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "Не удалось загрузить файл стилей:" << stylePath;
    }

    // Извещение всех подписчиков о смене темы
    emit themeChanged(m_currentTheme);
}

void ThemeManager::toggleTheme()
{
    if (m_currentTheme == "light") {
        setTheme("dark");
    } else {
        setTheme("light");
    }
}
