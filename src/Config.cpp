#include "Config.hpp"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFile>

std::unique_ptr<QSettings> Config::settings; // { QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() };

void Config::initialise(void)
{
    settings.reset(new  QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName()));
}

QPoint Config::getMainWindowPosition(void)
{
    return settings->value("mainWindow/pos", QPoint(0, 0)).toPoint();
}

void Config::saveMainWindowPosition(const QPoint & pos)
{
    settings->setValue("mainWindow/pos", pos);
}

QSize Config::getMainWindowSize(void)
{
    return settings->value("mainWindow/size", QSize(700, 600)).toSize();
}

void Config::saveMainWindowSize(const QSize & size)
{
    settings->setValue("mainWindow/size", size);
}

QString Config::getGlobalStylesheetPath(void)
{
    return settings->value("ui/stylesheet/global/path", ":/UI/default.uis").toString();
}

void Config::saveGlobalStylesheetPath(const QString & stylesheetPath)
{
    settings->setValue("ui/stylesheet/global/path", stylesheetPath);
}


QString Config::getGlobalStylesheet(void)
{
    if (!isUsingCustomStyleSheet())
        return "";

    auto path = Config::getGlobalStylesheetPath();
    QFile stylesheetFile(path);
    if (!stylesheetFile.exists())
    {
        qDebug() << "Stylesheet does not exist at [" << path << "]";
        return "";
    }

    if (!stylesheetFile.open(QFile::ReadOnly))
    {
        qDebug() << "Failure openning stylesheet at [" << path << "]";
        return "";
    }

    return stylesheetFile.readAll();
}

bool Config::isUsingCustomStyleSheet(void)
{
    return settings->value("ui/stylesheet/useCustom", false).toBool();
}

void Config::setIsUsingCustomStyleSheet(bool isUsingCustomStyleSheet)
{
    settings->setValue("ui/stylesheet/useCustom", isUsingCustomStyleSheet);
}

int Config::getLastMainWindowPlayerListTabWidgetIndex(void)
{
    return settings->value("mainWindow/currentPlayerListTabIndex", 0).toInt();
}

void Config::setLastMainWindowPlayerListTabWidgetIndex(int index)
{
    settings->setValue("mainWindow/currentPlayerListTabIndex", index);
}
