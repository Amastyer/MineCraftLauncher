#ifndef DREAMCRAFT_H
#define DREAMCRAFT_H

#include <QObject>
#include <QProcess>

class DreamCraft : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QString windowBgColor READ windowBgColor WRITE setWindowBgColor NOTIFY windowBgColorChanged)
public:
    explicit DreamCraft(QObject *parent = 0);
    ~DreamCraft();
    bool visible() { return m_visible; }
    QString windowBgColor() { return m_windowBgColor; }
signals:
    void visibleChanged();
    void windowBgColorChanged();

public slots:
    void setVisible( bool arg ) { m_visible = arg; emit visibleChanged(); return; }
    void setWindowBgColor( QString arg ) { m_windowBgColor = arg; emit windowBgColorChanged(); return; }
    QString test() { return "test"; }
    QString httpGet(QString url , int timeOut = 6000);
    QString httpPost(QString url, QString arg, int timeOut = 6000);
    void startGame(QString strPlayerName,
                   int nMaxMem,
                   QString strJavaPath,
                   QString strVersion ,
                   QString strClientDir,
                   int nUserId,
                   QString strAccessToken);
    void findMineCraftWindow();

private:
    bool m_visible;
    QString m_windowBgColor;
    QProcess *myProcess;
};

#endif // DREAMCRAFT_H
