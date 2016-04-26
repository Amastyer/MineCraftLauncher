#include "downloadthread.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QDir>
#include <QApplication>

DownloadThread::DownloadThread(QString localPath, QString remoteAddress, QObject *parent) : QThread(parent)
{
    m_localPath = localPath;
    m_remoteAddress = remoteAddress;
}

void DownloadThread::run()
{
    emit messageSignal( "检查文件更新" );
    emit statusSignal( "downloading" );

    // Step1 下载服务器md5文件
    int sta = downloadFile( m_remoteAddress + "/md5.txt", m_localPath + "/md5.txt" );
    if( sta != 0 ) {
        emit messageSignal( "获取服务器文件列表失败，请检查网络" );
        emit statusSignal( "unfinished" );
        return;
    }

    // Step2 获取本地文件md5列表
    QStringList listLocalFiles = findFiles( m_localPath );
    QMap<QString, QString> mapFilesMd5 = scanFilesMd5( listLocalFiles );

    // Step3 获取服务器文件md5列表
    QMap<QString, QString> mapMd5 = readMd5File( m_localPath + "/md5.txt" );

    // Step4 对比文件获取需要下载和删除的文件列表
    QStringList listDownloadFiles = getDownloadFileList( mapFilesMd5, mapMd5 );
    QStringList listRemoveFiles = getremoveFileList( mapFilesMd5, mapMd5 );

    if( listDownloadFiles.length() == 0 && listRemoveFiles.length() == 0 ) {
        emit messageSignal( "所有文件已是最新" );
        if( !QFile::exists( m_localPath + "/md5.txt.dc" ) )
            QFile::copy( m_localPath + "/md5.txt", m_localPath + "/md5.txt.dc" );

        if( QFile::exists( m_localPath + "/md5.txt.dc" ) )
            emit statusSignal( "finished" );
        else {
            emit messageSignal( "未知错误，请重试" );
            emit statusSignal( "unfinished" );
        }
        return;
    }

    // Step5 删除需要删除的文件
    emit messageSignal( "删除旧文件" );
    removeFiles( listRemoveFiles );

    // Step6 下载需要更新的文件
    emit messageSignal( "开始下载更新文件" );
    downloadFiles( listDownloadFiles );

    // Step7 校验下载
    emit messageSignal( "校验下载...." );
    checkDownload();

    // Step8 下载完成，更新md5.txt.dc
    emit messageSignal( "下载完成" );
    QFile::copy( m_localPath + "/md5.txt", m_localPath + "/md5.txt.dc" );
    if( QFile::exists( m_localPath + "/md5.txt.dc" ) )
        emit statusSignal( "finished" );
    else {
        emit messageSignal( "未知错误，请重试" );
        emit statusSignal( "unfinished" );
    }

}

void DownloadThread::downloadFiles(const QStringList listDownloadFiles)
{
    if( listDownloadFiles.length() == 0 ) return;
    for( int i = 0, max = listDownloadFiles.length(); i < max; i++ ) {
        QString strRemoteAddress = m_remoteAddress + "/" + listDownloadFiles[i];
        QString strLocalPath = m_localPath + "/" + listDownloadFiles[i];
        strLocalPath.replace("//", "/", Qt::CaseSensitive);
        emit messageSignal( QString("正在下载%1/%2").arg( i + 1 ).arg( max ) );
        qDebug() << downloadFile( strRemoteAddress, strLocalPath);
        emit currentSignal( i + 1 );
        emit totalSignal( max );
    }
    return;
}

void DownloadThread::checkDownload()
{
    downloadFile( m_remoteAddress + "/md5.txt", m_localPath + "/md5.txt" );
    QStringList listLocalFiles = findFiles( m_localPath );
    QMap<QString, QString> mapFilesMd5 = scanFilesMd5( listLocalFiles );
    QMap<QString, QString> mapMd5 = readMd5File( m_localPath + "/md5.txt" );
    QStringList listDownloadFiles = getDownloadFileList( mapFilesMd5, mapMd5 );
    QStringList listRemoveFiles = getremoveFileList( mapFilesMd5, mapMd5 );
    qDebug() << "删除列表:" << listRemoveFiles;
    if( listRemoveFiles.length() > 0 ) removeFiles( listRemoveFiles );
    if( listDownloadFiles.length() > 0 ) {
        downloadFiles( listDownloadFiles );
        checkDownload();
    }
    return;
}

void DownloadThread::removeFiles(const QStringList listRemoveFiles)
{
    if( listRemoveFiles.length() == 0 ) return;
    for( int i = 0, max = listRemoveFiles.length(); i < max; i++ ) {
        QFile::remove( listRemoveFiles[i] );
    }
}

QStringList DownloadThread::getDownloadFileList(QMap<QString, QString> mapFilesMd5, QMap<QString, QString> mapMd5)
{
    QStringList listFiles;
    QMap<QString, QString>::const_iterator i;
    for( i = mapMd5.constBegin(); i != mapMd5.constEnd(); i++ ) {
        if( !mapFilesMd5.contains( i.key() ) ) {
            listFiles.append( i.key() );
            qDebug() << i.key() << "不存在";
        }
        else {
            if( i.value() != mapFilesMd5.value(i.key()) ) {
                listFiles.append( i.key() );
            }
        }
    }
    return listFiles;
}

QStringList DownloadThread::getremoveFileList(QMap<QString, QString> mapFilesMd5, QMap<QString, QString> mapMd5)
{
    QStringList listFiles;
    QMap<QString, QString>::const_iterator i;
    for( i = mapMd5.constBegin(); i != mapMd5.constEnd(); i++ ) {
        if( mapFilesMd5.contains( i.key() ) ) {
            if( i.value() != mapFilesMd5.value(i.key()) ) {
                listFiles.append( i.key() );
            }
        }
    }

    for( i = mapFilesMd5.constBegin(); i != mapFilesMd5.constEnd(); i++ ) {
        if( !mapMd5.contains( i.key() ) ) listFiles.append( i.key() );
    }

    return listFiles;
}

int DownloadThread::downloadFile(const QString strRemoteAddress,
                                 const QString strLocalPath)
{
    qDebug() << strRemoteAddress << strLocalPath;
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(strRemoteAddress)));

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot( 5 * 60 * 1000, &loop, SLOT(quit()) );
    loop.exec();

    if( reply->error() ) {
        qDebug() << reply->errorString();
        return 1;
    }

    QBuffer fileBuffer;
    fileBuffer.open(QBuffer::Append);
    QByteArray data = reply->readAll();
    manager->deleteLater();
    reply->deleteLater();
    fileBuffer.write(data);
    fileBuffer.close();

    QFileInfo fileInfo( strLocalPath );
    QDir dir;
    if( !dir.exists( fileInfo.absolutePath() ) ) {
        dir.mkpath( fileInfo.absolutePath() );
    }

    QFile file(strLocalPath);
    if(!file.open(QFile::WriteOnly)) return 2;
    file.write(fileBuffer.data());
    file.close();
    return 0;
}

QString DownloadThread::getFileMd5(const QString &strFile)
{
    if( !QFile::exists(strFile) ) return QString();
    QFile file( strFile );
    file.open( QIODevice::ReadOnly );
    QByteArray ba = QCryptographicHash::hash( file.readAll() ,QCryptographicHash::Md5 );
    file.close();
    return ba.toHex().constData();
}

QMap<QString, QString> DownloadThread::scanFilesMd5(const QStringList listFiles)
{
    QMap<QString, QString> mapFilesMd5;
    Q_FOREACH( const QString strFilePath, listFiles ) {
        QString strMd5 = getFileMd5( strFilePath );
        if( mapFilesMd5.contains( strFilePath ) ) continue;
        QString filePath = strFilePath;
        filePath.replace(m_localPath, "");
        if( QString(filePath.at(1)) == "/" ) filePath.replace(0, 1, "");
        if( filePath == "md5.txt" || filePath == "md5.txt.dc" ) continue;
        mapFilesMd5.insert( filePath, strMd5 );
    }
    return mapFilesMd5;
}

QMap<QString, QString> DownloadThread::readMd5File(const QString strMd5File)
{
    QMap<QString, QString> mapMd5;
    QFile f( strMd5File );
    if( f.open(QFile::ReadOnly) == false ) return mapMd5;
    QStringList listFileData = QString( f.readAll() ).split("\n");
    for( int i = 0, max = listFileData.length(); i < max; i++ ) {
        QString strLine = listFileData[i];
        if( !strLine.contains("|") ) continue;
        QString strMd5 = strLine.split("|")[0];
        QString strFile = strLine.split("|")[1];
        if( mapMd5.contains( strFile ) ) continue;
        mapMd5.insert( strFile, strMd5 );
    }
    return mapMd5;
}

QStringList DownloadThread::findFiles(const QString &strDir)
{
    QStringList listFils;
    QDir dir( strDir );
    if( !dir.exists( strDir ) ) return QStringList();
    QFileInfoList listFileInfo = dir.entryInfoList( QDir::Dirs
                                                    | QDir::Files
                                                    | QDir::NoSymLinks
                                                    | QDir::Hidden
                                                    | QDir::NoDotAndDotDot,
                                                    QDir::Name );
    Q_FOREACH( QFileInfo fi, listFileInfo ) {
        if( fi.isSymLink() ) {}
        else if( fi.isDir() ) {
            QStringList listSubDirFiles = findFiles( fi.absoluteFilePath() );
            if( !listSubDirFiles.isEmpty() ) listFils.append( listSubDirFiles );
        }
        else if( fi.isFile() ) listFils.append( fi.absoluteFilePath() );
    }
    return listFils;
}

QStringList DownloadThread::findDirs(const QString &strDir)
{
    QStringList listFiles;
    QDir dir( strDir );
    if( !dir.exists( strDir ) ) return QStringList();
    QFileInfoList listFileInfo = dir.entryInfoList( QDir::Dirs
                                                    | QDir::Files
                                                    | QDir::Hidden
                                                    | QDir::NoSymLinks
                                                    | QDir::NoDotAndDotDot,
                                                    QDir::Name );
    Q_FOREACH( QFileInfo fi, listFileInfo ) {
        if( !fi.isDir() ) continue;
        listFiles.append( fi.absoluteFilePath() );
        QStringList listSubDirFiles = findDirs( fi.absoluteFilePath() );
        if( !listSubDirFiles.isEmpty() ) listFiles.append( listSubDirFiles );
    }
    return listFiles;
}
