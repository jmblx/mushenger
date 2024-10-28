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
    : m_fontScale(100)
    , m_fontFamily("Inter")
    , baseFontSizeSmall(14)
    , baseFontSizeMedium1(22)
    , baseFontSizeMedium2(26)
    , baseFontSizeLarge1(33)
    , baseFontSizeLarge2(46)
    , baseFontSizeXL(48)
    , baseFontSizeXXL(64)
{
    loadThemeFromSettings();
    loadFontSettings();
    loadAndProcessStylesheets();
    applyStyleSheet();
}

void ThemeManager::loadAndProcessStylesheets()
{
    QStringList themes = { "light", "dark" };
    for (const QString& theme : themes) {
        QString stylePath = QString(":/styles/%1.qss").arg(theme);
        QFile styleFile(stylePath);
        if (styleFile.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromUtf8(styleFile.readAll());
            styleFile.close();

            QString processedStyleSheet = processStyleSheet(styleSheet);

            cachedStylesheets.insert(theme, processedStyleSheet);
        } else {
            qDebug() << "Не удалось загрузить файл стилей:" << stylePath;
        }
    }
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

void ThemeManager::loadFontSettings()
{
    QSettings settings("IS32", "mushenger");
    m_fontFamily = settings.value("fontFamily", "Inter").toString();
    m_fontScale = settings.value("fontScale", 100).toInt();
}

void ThemeManager::saveFontSettings()
{
    QSettings settings("IS32", "mushenger");
    settings.setValue("fontFamily", m_fontFamily);
    settings.setValue("fontScale", m_fontScale);
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::setTheme(const QString& theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        saveThemeToSettings();
        applyStyleSheet();
        emit themeChanged(m_currentTheme);
    }
}

void ThemeManager::toggleTheme()
{
    if (m_currentTheme == "light") {
        setTheme("dark");
    } else {
        setTheme("light");
    }
}

void ThemeManager::setFontFamily(const QString& fontFamily)
{
    if (m_fontFamily != fontFamily) {
        m_fontFamily = fontFamily;
        saveFontSettings();
        loadAndProcessStylesheets();
        applyStyleSheet();
        emit fontFamilyChanged(m_fontFamily);
    }
}

QString ThemeManager::fontFamily() const
{
    return m_fontFamily;
}

void ThemeManager::setFontScale(int scalePercentage)
{
    if (m_fontScale != scalePercentage) {
        m_fontScale = scalePercentage;
        saveFontSettings();
        loadAndProcessStylesheets();
        applyStyleSheet();
        emit fontScaleChanged(m_fontScale);
    }
}

int ThemeManager::fontScale() const
{
    return m_fontScale;
}

void ThemeManager::applyStyleSheet()
{
    QString styleSheet = cachedStylesheets.value(m_currentTheme);
    if (!styleSheet.isEmpty()) {
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "No cached stylesheet found for theme:" << m_currentTheme;
    }
}

QString ThemeManager::processStyleSheet(const QString& styleSheet)
{
    QString processedStyleSheet = styleSheet;

    processedStyleSheet.replace("{font-family}", m_fontFamily);

    int sizeSmall = fontSizeSmall();
    int sizeMedium1 = fontSizeMedium1();
    int sizeMedium2 = fontSizeMedium2();
    int sizeLarge1 = fontSizeLarge1();
    int sizeLarge2 = fontSizeLarge2();
    int sizeXL = fontSizeXL();
    int sizeXXL = fontSizeXXL();

    processedStyleSheet.replace("{font-size-small}", QString::number(sizeSmall));
    processedStyleSheet.replace("{font-size-medium1}", QString::number(sizeMedium1));
    processedStyleSheet.replace("{font-size-medium2}", QString::number(sizeMedium2));
    processedStyleSheet.replace("{font-size-large1}", QString::number(sizeLarge1));
    processedStyleSheet.replace("{font-size-large2}", QString::number(sizeLarge2));
    processedStyleSheet.replace("{font-size-xl}", QString::number(sizeXL));
    processedStyleSheet.replace("{font-size-xxl}", QString::number(sizeXXL));

    return processedStyleSheet;
}

int ThemeManager::fontSizeSmall() const { return baseFontSizeSmall * m_fontScale / 100; }

int ThemeManager::fontSizeMedium1() const { return baseFontSizeMedium1 * m_fontScale / 100; }

int ThemeManager::fontSizeMedium2() const{ return baseFontSizeMedium2 * m_fontScale / 100; }

int ThemeManager::fontSizeLarge1() const { return baseFontSizeLarge1 * m_fontScale / 100; }

int ThemeManager::fontSizeLarge2() const { return baseFontSizeLarge2 * m_fontScale / 100; }

int ThemeManager::fontSizeXL() const { return baseFontSizeXL * m_fontScale / 100; }

int ThemeManager::fontSizeXXL() const { return baseFontSizeXXL * m_fontScale / 100; }
