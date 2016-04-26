#include <QApplication>
#include <QQmlApplicationEngine>
#include <dreamcraft.h>
#include <dcsettings.h>
#include <downloadprocess.h>
#include <dcsystemtrayicon.h>
#include <QMessageBox>
#include <QDesktopServices>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("DreamCraft");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Amast");
    app.setApplicationDisplayName("梦世界");

    DcSettings settings;
    if( settings.javaPath().isEmpty() ) {
        QString javaPath = settings.getSystemJavaPath();
        if( javaPath.isEmpty() ) {  // 如果java未安装，提示用户下载
            QMessageBox message(QMessageBox::NoIcon,
                                "Java未安装提示", "MC需要Java才能正常运行，检测到您并未正常安装Java，是否转到Java官网进行下载?",
                                QMessageBox::Yes | QMessageBox::No, NULL);

            if( message.exec() == QMessageBox::Yes ) QDesktopServices::openUrl(QUrl("http://www.java.com"));
        } else {
            settings.setJavaPath(javaPath);
        }
    }

    QQmlApplicationEngine engine;
    qmlRegisterType<DcSettings>("DcSettings", 0, 1, "DcSettings");
    qmlRegisterType<DownloadProcess>("DownloadProcess", 0, 1, "DownloadProcess");
    engine.rootContext()->setContextProperty("g_DreamCraft", new DreamCraft);
    engine.rootContext()->setContextProperty("g_DcSettings", &settings);
    engine.rootContext()->setContextProperty("g_DcSystemTaryIcon", new DcSystemTrayIcon);
    engine.load(QUrl(QStringLiteral("file:qml/main.qml")));
    return app.exec();
}
