#ifndef DCSYSTEMTRAYICON_H
#define DCSYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QDebug>

class DcSystemTrayIcon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLogout READ isLogout WRITE setIsLogout NOTIFY isLogoutChanged)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged)
public:
    explicit DcSystemTrayIcon(QObject *parent = 0);
    bool isLogout() { return m_isLogout; }
    bool isVisible() { return m_isVisible; }

signals:
    void isLogoutChanged();
    void isVisibleChanged();

public slots:
    void setIsLogout( bool arg ) { m_isLogout = arg; emit isLogoutChanged(); qDebug() << arg; return; }
    void setIsVisible( bool arg ) { m_isVisible = arg; emit isVisibleChanged(); return; }
    void setLogoutActionVisible( bool visible ) { m_logoutAction->setVisible( visible ); return; }

private slots:
    void quitActionSlot() { qApp->quit(); }
    void showActionSlot() { setIsVisible( !m_isVisible ); }
    void logoutActionSlot() { setIsLogout( !m_isLogout ); }

private:
    bool m_isLogout = false;
    bool m_isVisible = false;
    QSystemTrayIcon *m_systemTaryIcon;
    QAction *m_showAction;
    QAction *m_logoutAction;
    QAction *m_quitAction;
    QMenu *m_taryIconMenu;
};

#endif // DCSYSTEMTRAYICON_H
