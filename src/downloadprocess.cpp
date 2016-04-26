#include "downloadprocess.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QApplication>
#include "downloadthread.h"

DownloadProcess::DownloadProcess(QObject *parent) : QObject(parent)
{
    m_message = "";
    m_current = 0;
    m_total = 1;
    downloadThread = new DownloadThread;
    connect( downloadThread, SIGNAL( messageSignal( QString ) ), this, SLOT( setMessage( QString ) ) );
    connect( downloadThread, SIGNAL( currentSignal( int ) ), this, SLOT( setCurrent( int ) ) );
    connect( downloadThread, SIGNAL( totalSignal( int ) ), this, SLOT( setTotal( int ) ) );
    connect( downloadThread, SIGNAL( statusSignal( QString ) ), this, SLOT( setStatus( QString ) ) );
}

void DownloadProcess::downloadProgressSlot(int current, int total)
{
    setCurrent( current );
    setTotal( total );
    return;
}

void DownloadProcess::checkClientStatus( QString strMd5 )
{
    QString strApplicationDirPath = QApplication::applicationDirPath();
    QString strLocalMd5File = strApplicationDirPath + "/" + m_localPath + "/md5.txt";
    QString strRemoteMd5File = strApplicationDirPath + "/" + m_localPath + "/rmd5.txt";
    QString strFinishMd5File = strApplicationDirPath + "/" + m_localPath + "/md5.txt.dc";
    qDebug() << QFile::exists(strLocalMd5File) << strLocalMd5File;
    if( QFile::exists(strFinishMd5File) ) {
        QFile file( strFinishMd5File );
        file.open( QIODevice::ReadOnly );
        QByteArray ba = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
        file.close();
        qDebug() << ba.toHex().constData() << strMd5;
        if( ba.toHex().constData() == strMd5 ) {
            setStatus("finished");
            return;
        } else {
            setStatus("unfinished");
            return;
        }
    }
    if( QFile::exists(strFinishMd5File) ) setStatus("finished");
    else if( QFile::exists(strRemoteMd5File) ) setStatus("unfinished");
    else if( !QFile::exists(strLocalMd5File)  ) setStatus("empty");
    else setStatus("empty");
    return;
}

void DownloadProcess::startDownload()
{
    if( m_status == "downloading" ) return;
    QString strLocalPath = QApplication::applicationDirPath() + "/" + m_localPath;
    downloadThread->setLocalPath( strLocalPath );
    downloadThread->setRemoteAddress( m_remoteAddress );
    downloadThread->start();
}
