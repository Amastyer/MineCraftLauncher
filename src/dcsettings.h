#ifndef DCSETTINGS_H
#define DCSETTINGS_H

#include <QObject>

class DcSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged )
    Q_PROPERTY( bool autoLogin READ autoLogin WRITE setAutoLogin NOTIFY autoLoginChanged )
    Q_PROPERTY( bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged )
    Q_PROPERTY( bool rememberPassword READ rememberPassword WRITE setRememberPassword NOTIFY rememberPasswordChanged)
    Q_PROPERTY( int maxMem READ maxMem WRITE setMaxMem NOTIFY maxMemChanged )
    Q_PROPERTY( QString javaPath READ javaPath WRITE setJavaPath NOTIFY javaPathChanged )
    Q_PROPERTY( QString email READ email WRITE setEmail NOTIFY emailChanged )
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )
public:
    explicit DcSettings(QObject *parent = 0);
    bool autoStart() { return m_autoStart; }
    bool autoLogin() { return m_autoLogin; }
    bool autoUpdate() { return m_autoUpdate; }
    bool rememberPassword() { return m_rememberPassword; }
    int maxMem() { return m_maxMem; }
    QString javaPath() { return m_javaPath; }
    QString email() { return m_email; }
    QString password() { return m_password; }

signals:
    void autoStartChanged();
    void autoLoginChanged();
    void autoUpdateChanged();
    void maxMemChanged();
    void javaPathChanged();
    void emailChanged();
    void passwordChanged();
    void rememberPasswordChanged();

public slots:
    int getSystemMemory();
    QString getSystemJavaPath();
    void setAutoStart( bool isAutoStart );
    void setAutoLogin( bool isAutoLogin );
    void setAutoUpdate( bool isAutoUpdate );
    void setRememberPassword( bool isRememberPassword );
    void setMaxMem( int intMaxMem );
    void setJavaPath( QString strJavaPath );
    void setEmail( QString strEmail );
    void setPassword( QString strPassword );
    bool is64BitSystem();
    bool is64BitJava();

private:
    bool m_autoStart;
    bool m_autoLogin;
    bool m_autoUpdate;
    bool m_rememberPassword;
    int m_maxMem;
    QString m_javaPath;
    QString m_email;
    QString m_password;

    int readIntSetting( QString path, QString key );
    bool readBoolSetting( QString path, QString key );
    QString readQStringSetting( QString path, QString key );
    bool writeSetting( QString path, QString key, int value );
    bool writeSetting( QString path, QString key, bool value );
    bool writeSetting( QString path, QString key, QString value );
};

#endif // DCSETTINGS_H
