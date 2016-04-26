import QtQuick 2.3
import "Login"
import "MainPanel"
import QtQuick.Window 2.0
//import "DcApi.js"

Window {
    id: root
    width: 366
    height: 514
    color: g_DreamCraft.windowBgColor
    visible: g_DreamCraft.visible
    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    flags: Qt.FramelessWindowHint | Qt.Window

    property string m_strApiAddress: "http://play.i5mc.com:10010/api"
    property string m_nUserId
    property string m_strAccessToken: ""
    property string m_strUsername: ""
    property string m_strNoavatar: ""
    property string m_strEmail: g_DcSettings.email
    property string m_strPassword: g_DcSettings.password
    property bool m_bAutoLogin: g_DcSettings.autoLogin
    property bool m_bRememberPassword: g_DcSettings.rememberPassword
    property string m_strJavaPath: g_DcSettings.javaPath
    property string m_strPlayerName: "Amast"
    property int m_nMaxMem
    property bool isVisible: g_DcSystemTaryIcon.isVisible
    property bool isLogout: g_DcSystemTaryIcon.isLogout
    onIsVisibleChanged: show()
    onIsLogoutChanged: logout()

    MouseArea {
        anchors.fill: parent
        property point clickPos
        onPressed: clickPos = Qt.point( mouseX, mouseY )
        onPositionChanged: {
            var pos = Qt.point( mouse.x - clickPos.x, mouse.y - clickPos.y );
            root.setX( root.x + pos.x );
            root.setY( root.y + pos.y );
        }
    }

    Loader { id: loginWindowLoader }
    Loader { id: mainPanelWindowLoader }
    Connections { target: loginWindowLoader.item; onLogined: showMainPanelWindow() }
    Component.onCompleted: showLoginWindow()
//    Component.onCompleted: showMainPanelWindow()

    function showMainPanelWindow() {
        loginWindowLoader.setSource("");
        root.width = 1280;
        root.height = 720;
        mainPanelWindowLoader.source = "MainPanel/MainPanel.qml";
        g_DcSystemTaryIcon.setLogoutActionVisible( true );
    }

    function showLoginWindow() {
        mainPanelWindowLoader.setSource("");
        root.width = 366;
        root.height = 514;
        loginWindowLoader.source = "Login/LoginPanel.qml";
        g_DcSystemTaryIcon.setLogoutActionVisible( false );
    }

    function logout() {
        m_nUserId = 0;
        m_strAccessToken = "";
        m_strNoavatar = "";
        m_strUsername = "";
        showLoginWindow();
    }
}
