#include "dcsystemtrayicon.h"

DcSystemTrayIcon::DcSystemTrayIcon(QObject *parent) :
    QObject(parent)
{
    m_showAction = new QAction( "显示主界面", this );
    m_logoutAction = new QAction( "注销登陆", this );
    m_quitAction = new QAction( "退出程序", this );
    m_taryIconMenu = new QMenu;
    m_taryIconMenu->addAction( m_showAction );
    m_taryIconMenu->addAction( m_logoutAction );
    m_taryIconMenu->addAction( m_quitAction );
    m_systemTaryIcon = new QSystemTrayIcon;
    qDebug() << QApplication::applicationDirPath() + "/icon.ico";
    m_systemTaryIcon->setIcon( QIcon( QApplication::applicationDirPath() + "/icon.ico" ) );
    m_systemTaryIcon->setContextMenu( m_taryIconMenu );
    m_systemTaryIcon->show();
    connect( m_showAction, SIGNAL( triggered() ), this, SLOT( showActionSlot() ) );
    connect( m_logoutAction, SIGNAL( triggered() ), this, SLOT( logoutActionSlot() ) );
    connect( m_quitAction, SIGNAL( triggered() ), this, SLOT( quitActionSlot() ) );
}
