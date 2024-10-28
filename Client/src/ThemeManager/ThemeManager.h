#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager& instance();
    QString currentTheme() const;

    void setTheme(const QString& theme);
    void toggleTheme();

    void setFontScale(int scalePercentage);
    int fontScale() const;

    void setFontFamily(const QString& fontFamily);
    QString fontFamily() const;

signals:
    void themeChanged(const QString& newTheme);
    void fontScaleChanged(int newScale);
    void fontFamilyChanged(const QString& newFontFamily);

private:
    ThemeManager();
    QString m_currentTheme;

    int m_fontScale;
    QString m_fontFamily;

    int baseFontSizeSmall;
    int baseFontSizeMedium1;
    int baseFontSizeMedium2;
    int baseFontSizeLarge1;
    int baseFontSizeLarge2;
    int baseFontSizeXL;
    int baseFontSizeXXL;

    void loadThemeFromSettings();
    void saveThemeToSettings();

    void loadFontSettings();
    void saveFontSettings();

    void applyStyleSheet();

    QString processStyleSheet(const QString& styleSheet);

    int fontSizeSmall() const;
    int fontSizeMedium1() const;
    int fontSizeMedium2() const;
    int fontSizeLarge1() const;
    int fontSizeLarge2() const;
    int fontSizeXL() const;
    int fontSizeXXL() const;

    QHash<QString, QString> cachedStylesheets;
    void loadAndProcessStylesheets();
};

#endif // THEMEMANAGER_H
