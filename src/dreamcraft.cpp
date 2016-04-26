#include <dreamcraft.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

DreamCraft::DreamCraft(QObject *parent) : QObject(parent)
{
    m_visible = true;
#ifdef Q_OS_WIN
    OSVERSIONINFO osVersion;
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osVersion);
    float nVersion = osVersion.dwMajorVersion + osVersion.dwMinorVersion * 0.1;
    if( nVersion < 6 ) {
        m_windowBgColor = "white";
    } else {
        m_windowBgColor = "transparent";
    }
#else
    m_windowBgColor = "transparent";
#endif
    QTimer *timer = new QTimer(this);
    timer->setInterval( 1000 );
    timer->start();

    myProcess = new QProcess( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( findMineCraftWindow() ) );
}

DreamCraft::~DreamCraft()
{
    qDebug() << "开始析构";
    myProcess->close();
    myProcess->deleteLater();
}

void DreamCraft::findMineCraftWindow()
{
    HWND mainWnd, mcWnd;
    QString ClassName( "LWJGL" );
    QString AppName( "DreamCraft" );
    LPCWSTR App = reinterpret_cast < LPCWSTR >( ClassName.data() );
    LPCWSTR AppCaption = reinterpret_cast <LPCWSTR>(AppName.data());
    mainWnd = ::FindWindow( NULL, AppCaption );
    mcWnd= ::FindWindow( App, NULL);
    if( mainWnd && mcWnd ) {
        RECT mainRect, mcRect;
        GetWindowRect(mainWnd, &mainRect);
        GetWindowRect(mcWnd, &mcRect);
        if( mcRect.right - mcRect.left < mainRect.right - mainRect.left && mcRect.bottom - mcRect.top < mainRect.bottom - mcRect.top ) {
            SetParent( mcWnd, mainWnd );
            LONG_PTR Style = ::GetWindowLongPtr( mcWnd, GWL_STYLE);
            Style = Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX;
            ::SetWindowLongPtr(mcWnd, GWL_STYLE, Style);
            int x = 0;
            int y = 71;
            MoveWindow( mcWnd, x, y, 974, 520, false );
        } else {
            qDebug() << "判断是否父子关系" << IsChild( mainWnd, mcWnd);
            if( IsChild( mainWnd, mcWnd ) ) {
                qDebug() << "父子关系";
            }
        }
    }
    return;
}

void DreamCraft::startGame(QString strPlayerName,
                           int nMaxMem,
                           QString strJavaPath,
                           QString strVersion,
                           QString strClientDir,
                           int nUserId,
                           QString strAccessToken)
{
    if( nMaxMem < 512 ) nMaxMem = 512;
    if( !QFile::exists(strJavaPath) ) {
        QMessageBox::warning( NULL,
                              "错误提示",
                              QString( "启动游戏失败，未在%1找到Java程序" ).arg( strJavaPath ),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes );
        return;
    }

    QString strCurrentDir = QApplication::applicationDirPath() + "/" + strClientDir;
    QString strLoadFile = ( QApplication::applicationDirPath()
                            + "/%1/.minecraft/versions/%2/%2.json" )
                            .arg( strClientDir )
                            .arg( strVersion );

    QFile fLoad( strLoadFile );
    if( !fLoad.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning( NULL,
                              "错误提示",
                              QString( "启动游戏失败，读取%1失败" ).arg( strLoadFile ),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes );
        return;
    }

    QByteArray bSaveData = fLoad.readAll();
    fLoad.close();

    QJsonParseError jsonError;
    QStringList classPathList;
    QString strLaunchArg, strMainClass, strVersionId, strAssets;
    QJsonDocument jsonLoadDoc( QJsonDocument::fromJson( bSaveData, &jsonError ) );
    if( jsonError.error == QJsonParseError::NoError ) {
        if( jsonLoadDoc.isObject() ) {
            QJsonObject obj = jsonLoadDoc.object();

            QJsonValue launchArgJsonValue = obj.take("minecraftArguments");
            strLaunchArg = launchArgJsonValue.toString();

            QJsonValue mainClassJsonValue = obj.take("mainClass");
            strMainClass = mainClassJsonValue.toString();

            QJsonValue versionIDJsonValue = obj.take("id");
            strVersionId = versionIDJsonValue.toString();

            QJsonValue assetsJsonValue = obj.take("assets");
            strAssets = assetsJsonValue.toString();

            QJsonValue libJsonValue = obj.take("libraries");
            QJsonArray libJsonArray = libJsonValue.toArray();

            for( int i = 0; i < libJsonArray.size(); i++ ){
                QJsonValue subLibJsonValue = libJsonArray.at(i);
                QJsonObject subLibJsonObj = subLibJsonValue.toObject();

                if( subLibJsonObj.contains("natives") ) {
                    QJsonValue libNatives = subLibJsonObj.take("natives");
                    QJsonObject nativeJsonObj = libNatives.toObject();
                    QJsonValue nativeJsonValue = nativeJsonObj.take("windows");
                    QString nativeFileName = nativeJsonValue.toString();
                    QJsonValue nativeName = subLibJsonObj.take("name");
                    QString nativeNameStr = nativeName.toString();
                    QString nativeDirName = nativeNameStr.section(":",1,1)
                            + "-"
                            + nativeNameStr.section(":",2,2)
                            + "-"
                            + nativeFileName
                            + ".jar";
                    QStringList nativeDirList= nativeNameStr.split(":");
                    QString nativeDir = nativeDirList.at(0);
                    nativeDir = nativeDir.replace(".","/",Qt::CaseSensitive);
                    nativeDir = "/.minecraft/libraries/" + nativeDir+"/"+nativeDirName;
                    classPathList << nativeDir;
                } else {
                    QJsonValue libNameJsonValue = subLibJsonObj.take("name");
                    QString nameStr = libNameJsonValue.toString();
                    QString libFileName = nameStr.section(":",1,1)
                            + "/"+nameStr.section(":",2,2)
                            + "/"+nameStr.section(":",1,1)
                            + "-"
                            + nameStr.section(":",2,2)
                            + ".jar";
                    QStringList libFileDirList= nameStr.split(":");
                    QString libFileDir = libFileDirList.at(0);
                    libFileDir = libFileDir.replace(".","/",Qt::CaseSensitive);
                    libFileDir = libFileDir+"/"+libFileName;
                    libFileDir = ".minecraft/libraries/"+libFileDir;
                    classPathList << libFileDir;
                }
            }
        }
    } else {
        QMessageBox::warning( NULL,
                              "错误提示",
                              QString( "启动游戏失败，%1文件格式错误" ).arg( strLoadFile ),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes);
        return;
    }

    QStringList arguments;
    arguments << QString( "-Xmx%1m" ).arg( QString::number( nMaxMem ) );
    arguments << "-Dfml.ignoreInvalidMinecraftCertificates=true";
    arguments << "-Dfml.ignorePatchDiscrepancies=true";
    arguments << "-Dmc.launcher.uid=" + QString::number( nUserId );
    arguments << "-Dmc.launcher.token=" + strAccessToken;
    arguments << QString( "-Djava.library.path=%1.minecraft/versions/i5mc/i5mc-natives" ).arg( strCurrentDir );
    classPathList << QString( ".minecraft/versions/%1/%1.jar" ).arg( strVersion );

    QString classpath;
    for(int i=0;i<classPathList.size();i++){
        classpath += ( strCurrentDir + classPathList.at(i));
        if(i < classPathList.size() - 1) classpath += ";";
    }

    arguments<<"-cp";
    arguments<<classpath;
    arguments<<strMainClass;

    QStringList launchStrList = strLaunchArg.split(" ");
    for( int i = 0; i<launchStrList.size(); ){
        QString tmp = launchStrList.at( i );
        if( tmp.contains( "--username", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, strPlayerName );
        else if( tmp.contains( "--version", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, strVersionId );
        else if( tmp.contains( "--assetIndex", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, strAssets );
        else if( tmp.contains( "--gameDir", Qt::CaseSensitive)  ) launchStrList.replace( i + 1, strCurrentDir + "/.minecraft" );
        else if( tmp.contains( "--assetsDir", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, strCurrentDir + "/.minecraft/assets" );
        else if( tmp.contains( "--userProperties", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, "{}" );
        else if( tmp.contains( "--userType", Qt::CaseSensitive ) ) launchStrList.replace( i + 1, "legacy" );
        i += 2;
    }

    arguments += launchStrList;
    arguments << "--width" << "974";
    arguments << "--height" << "520";

    qDebug() << arguments;

    QStringList listEnv = QProcess::systemEnvironment();
    myProcess->setEnvironment( listEnv );
    myProcess->setProcessChannelMode( QProcess::MergedChannels );
    myProcess->start( strJavaPath, arguments );
    return;
}

QString DreamCraft::httpGet(QString url, int timeOut)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->get(req);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(timeOut,&loop,SLOT(quit()));
    loop.exec();
    QString strResponse = reply->readAll();
    return strResponse;
}

QString DreamCraft::httpPost(QString url, QString arg, int timeOut)
{
    QByteArray content = arg.toLatin1();
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, content.length());
    QNetworkReply *reply = manager->post(req, content);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(timeOut,&loop,SLOT(quit()));
    loop.exec();
    QString strResponse = reply->readAll();
    return strResponse;
}
