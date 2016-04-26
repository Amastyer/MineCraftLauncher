#ifndef DOWNLOADTHREAD_H
#define DOWNLOADTHREAD_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QNetworkReply>
#include <QBuffer>

class DownloadThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadThread(QString localPath = "",
                            QString remoteAddress = "",
                            QObject *parent = 0);
    virtual void run();
    void setLocalPath( const QString strLocalPath ) { m_localPath = strLocalPath; return; }
    void setRemoteAddress( const QString strRemoteAddress ) { m_remoteAddress = strRemoteAddress; return; }

signals:
    void messageSignal( QString message );
    void currentSignal( int current );
    void totalSignal( int total );
    void statusSignal( QString status );

private:
    QString m_remoteAddress;
    QString m_localPath;

    int downloadFile(const QString strRemoteAddress,
                     const QString strLocalPath);
    QString getFileMd5(const QString &strFile);
    QMap<QString, QString> scanFilesMd5(const QStringList listFiles);
    QMap<QString, QString> readMd5File(const QString strMd5File);
    QStringList findFiles(const QString &strDir);
    QStringList findDirs(const QString &strDir);
    QStringList getDownloadFileList( QMap<QString, QString> mapFilesMd5, QMap<QString, QString> mapMd5 );
    QStringList getremoveFileList( QMap<QString, QString> mapFilesMd5, QMap<QString, QString> mapMd5 );
    void downloadFiles( const QStringList listDownloadFiles );
    void removeFiles( const QStringList listRemoveFiles );
    void checkDownload();
};

#endif // DOWNLOADTHREAD_H
