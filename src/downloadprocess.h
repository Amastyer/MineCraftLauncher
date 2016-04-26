#ifndef DOWNLOADPROCESS_H
#define DOWNLOADPROCESS_H

#include <QObject>
#include <downloadthread.h>

class DownloadProcess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString remoteAddress READ remoteAddress WRITE setRemoteAddress NOTIFY remoteAddressChanged)
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
public:
    explicit DownloadProcess(QObject *parent = 0);
    QString status() { return m_status; }
    QString localPath() { return m_localPath; }
    QString remoteAddress() { return m_remoteAddress; }
    int current() { return m_current; }
    int total() { return m_total; }
    QString message() { return m_message; }

signals:
    void statusChanged();
    void localPathChanged();
    void remoteAddressChanged();
    void currentChanged();
    void totalChanged();
    void messageChanged();

public Q_SLOTS:
    void setStatus( QString arg ) { m_status = arg; emit statusChanged(); return; }
    void setLocalPath( QString arg ) { m_localPath = arg; emit localPathChanged(); return; }
    void setRemoteAddress( QString arg ) { m_remoteAddress = arg; emit remoteAddressChanged(); return; }
    void setCurrent( int arg ) { m_current = arg; emit currentChanged(); return; }
    void setTotal( int arg ) { m_total = arg; emit totalChanged(); return; }
    void setMessage( QString arg ) { m_message = arg; emit messageChanged(); return; }

public slots:
    void checkClientStatus( QString strMd5 );
    void startDownload();
    void downloadProgressSlot( int current, int total );

private:
    QString m_message;
    QString m_status;
    QString m_localPath;
    QString m_remoteAddress;
    int m_current;
    int m_total;
    DownloadThread *downloadThread;
};

#endif // DOWNLOADPROCESS_H
