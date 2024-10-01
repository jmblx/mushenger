#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager& instance();
    QString currentTheme() const;

    void setTheme(const QString& theme);
    void toggleTheme(); // Переключение темы

signals:
    void themeChanged(const QString& newTheme);

private:
    ThemeManager();
    QString m_currentTheme;

    void loadThemeFromSettings();
    void saveThemeToSettings();
};

#endif // THEMEMANAGER_H
