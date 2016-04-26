#include "dcsettings.h"
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QFile>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"      // 注册表路径
#define DEFAULT_SETTING_PATH (QApplication::applicationDirPath() + "/settings/config.ini") // 配置文件路径

DcSettings::DcSettings(QObject *parent) : QObject(parent)
{
    qDebug() << is64BitSystem();
    m_maxMem = readIntSetting( DEFAULT_SETTING_PATH, "maxMem" );
    m_javaPath = readQStringSetting( DEFAULT_SETTING_PATH, "javaPath" );
    m_autoStart = readBoolSetting( DEFAULT_SETTING_PATH, "autoStart" );
    m_autoLogin = readBoolSetting( DEFAULT_SETTING_PATH, "autoLogin" );
    m_autoUpdate = readBoolSetting( DEFAULT_SETTING_PATH, "autoUpdate" );
    m_email = readQStringSetting( DEFAULT_SETTING_PATH, "email" );
    m_password = readQStringSetting( DEFAULT_SETTING_PATH, "password" );
    m_rememberPassword = readBoolSetting( DEFAULT_SETTING_PATH, "rememberPassword" );
}

bool DcSettings::is64BitJava()
{
    QProcess p(0);
    QString javaExePath = m_javaPath;
    javaExePath.replace( "javaw", "java1", Qt::CaseInsensitive );
    p.start( javaExePath, QStringList() << "-version" );
    p.waitForFinished();
    QString data = QString::fromLocal8Bit(p.readAllStandardError());
    if( data.contains( "64-Bit" ) ) return true;
    return false;
}

bool DcSettings::is64BitSystem()
{
#ifdef Q_OS_WIN
    bool is64Bit = GetSystemWow64Directory;
    if( is64Bit ) return true;
    else return false;
#endif
    return false;
}

int DcSettings::getSystemMemory()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof( statusex );
    GlobalMemoryStatusEx( &statusex );
    return statusex.ullTotalPhys / (1024 * 1024);
#else
    return 4096;
#endif
}

// +------------------------------------------------------------------------
// |                            检查Java是否安装
// +------------------------------------------------------------------------
QString DcSettings::getSystemJavaPath()
{
    QString javaPath;
    QStringList listPath;
    QStringList environment = QProcess::systemEnvironment();
    foreach(QString str, environment) {
        if ( str.startsWith("PATH=", Qt::CaseInsensitive ) ) {
            listPath = str.split(";");
            break;
        }
    }
    if( listPath.length() == 0 ) return QString();
    qDebug() << listPath;
    foreach (QString str, listPath) {
        if( str.contains( "java", Qt::CaseInsensitive ) ) {
            qDebug() << "javaPath: " << str;
            str.replace( "path=", "", Qt::CaseInsensitive );
            javaPath = str + "\\javaw.exe";
            if( QFile::exists( javaPath ) ) break;
            javaPath = str + "\\java.exe";
            if( QFile::exists( javaPath ) ) break;
            javaPath = "";
        }
    }
    if( javaPath.isEmpty() ) return QString();
    return javaPath;
}

// +------------------------------------------------------------------------
// |                            读取配置信息
// +------------------------------------------------------------------------
int DcSettings::readIntSetting(QString path, QString key)
{
    if( path.isEmpty() || key.isEmpty() ) return int();
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    int value = settings->value( QString( "config/" ) + key ).toInt();
    settings->deleteLater();
    return value;
}

bool DcSettings::readBoolSetting(QString path, QString key)
{
    if( path.isEmpty() || key.isEmpty() ) return bool();
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    bool value = settings->value( QString( "config/" ) + key ).toBool();
    settings->deleteLater();
    return value;
}

QString DcSettings::readQStringSetting( QString path, QString key )
{
    if( path.isEmpty() || key.isEmpty() ) return QString();
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    QString value = settings->value( QString( "config/" ) + key ).toString();
    settings->deleteLater();
    return value;
}

// +------------------------------------------------------------------------
// |                            写入配置信息
// +------------------------------------------------------------------------
bool DcSettings::writeSetting( QString path, QString key, int value )
{
    if( path.isEmpty() || key.isEmpty() ) return false;
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    settings->beginGroup("config");
    settings->setValue( key, value );
    settings->endGroup();
    settings->deleteLater();
    return true;
}

bool DcSettings::writeSetting( QString path, QString key, bool value )
{
    if( path.isEmpty() || key.isEmpty() ) return false;
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    settings->beginGroup("config");
    settings->setValue( key, value );
    settings->endGroup();
    settings->deleteLater();
    return true;
}

bool DcSettings::writeSetting( QString path, QString key, QString value )
{
    if( path.isEmpty() || key.isEmpty() ) return false;
    QSettings *settings = new QSettings( path, QSettings::IniFormat );
    settings->beginGroup("config");
    settings->setValue( key, value );
    settings->endGroup();
    settings->deleteLater();
    return true;
}

// +------------------------------------------------------------------------
// |                            设置记住密码
// +------------------------------------------------------------------------
void DcSettings::setRememberPassword( bool isRememberPassword )
{
    if( m_rememberPassword == isRememberPassword ) return;
    m_rememberPassword = isRememberPassword;
    writeSetting( DEFAULT_SETTING_PATH, "rememberPassword", isRememberPassword );
    emit rememberPasswordChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置用户密码
// +------------------------------------------------------------------------
void DcSettings::setPassword( QString strPassword )
{
    if( m_password == strPassword ) return;
    m_password = strPassword;
    writeSetting( DEFAULT_SETTING_PATH, "password", strPassword );
    emit passwordChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置用户邮箱
// +------------------------------------------------------------------------
void DcSettings::setEmail( QString strEmail )
{
    if( m_email == strEmail ) return;
    m_email = strEmail;
    writeSetting( DEFAULT_SETTING_PATH, "email", strEmail );
    emit emailChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置自动登录
// +------------------------------------------------------------------------
void DcSettings::setAutoLogin( bool isAutoLogin )
{
    if( m_autoLogin == isAutoLogin ) return;
    m_autoLogin = isAutoLogin;
    writeSetting( DEFAULT_SETTING_PATH, "autoLogin", isAutoLogin );
    emit autoLoginChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置开机启动
// +------------------------------------------------------------------------
void DcSettings::setAutoStart( bool isAutoStart )
{
    if( m_autoStart == isAutoStart ) return;
    m_autoStart = isAutoStart;
    QString applicationName = QApplication::applicationName();
    QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
    if( isAutoStart ) {
        QString applicationPath = QApplication::applicationFilePath();
        settings->setValue(applicationName, applicationPath.replace("/", "\\"));
    } else {
        settings->remove(applicationName);
    }
    settings->deleteLater();
    writeSetting( DEFAULT_SETTING_PATH, "autoStart", isAutoStart );
    emit autoStartChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置自动更新
// +------------------------------------------------------------------------
void DcSettings::setAutoUpdate( bool isAutoUpdate )
{
    if( m_autoUpdate == isAutoUpdate ) return;
    m_autoUpdate = isAutoUpdate;
    writeSetting( DEFAULT_SETTING_PATH, "autoUpdate", isAutoUpdate );
    emit autoUpdateChanged();
    return;
}

// +------------------------------------------------------------------------
// |                         设置Java虚拟机最大内存
// +------------------------------------------------------------------------
void DcSettings::setMaxMem( int intMaxMem )
{
    if( m_maxMem == intMaxMem ) return;
    m_maxMem = intMaxMem;
    writeSetting( DEFAULT_SETTING_PATH, "maxMem", intMaxMem );
    emit maxMemChanged();
    return;
}

// +------------------------------------------------------------------------
// |                            设置Java程序路径
// +------------------------------------------------------------------------
void DcSettings::setJavaPath( QString strJavaPath )
{
    if( m_javaPath == strJavaPath ) return;
    m_javaPath = strJavaPath;
    writeSetting( DEFAULT_SETTING_PATH, "javaPath", strJavaPath );
    emit javaPathChanged();
    return;
}



