import QtQuick 2.0
import "../MyCom"
import QtQuick.Window 2.0
import "./DownloadPage"
import "./ServerPage"
import "./FriendPage"
import "./SettingPage"

Item {
    id: root
    width: 1280
    height: 720
    property var m_listServerArr: []

    Image { anchors.fill: parent; source: "../images/bg.jpg" }

    HeadPanel {
        id: headPanel
        onMinBtnClicked: showMinimized()
        onCloseBtnClicked: Qt.quit()
        onSetBtnClicked: settingPage.visible = settingPage.visible ? false : true
        onLogoClicked: {
            serverList.visible = true
            serverPage.visible = true
            settingPage.visible = false
        }
    }

    Item {
        width: parent.width
        height: parent.height - headPanel.height
        anchors.top: headPanel.bottom

        Item {
            id: serverList
            width: parent.width - friendPage.width
            height: parent.height

            ServerPage {
                id: serverPage
                anchors.fill: parent
                onSelect: {
                    visible = false
                    downloadPage.select( name )
                }
            }

            DownloadPage {
                id: downloadPage
                visible: serverPage.visible ? false : true
                anchors.fill: parent
            }
        }

        SettingPage {
            id: settingPage
            width: parent.width - friendPage.width
            height: parent.height
            visible: serverList.visible ? false : true
            onVisibleChanged: serverList.visible = visible ? false : true
        }

        FriendPage {
            id: friendPage
            anchors.right: parent.right
        }
    }

    Component.onCompleted: {
        var url =  m_strApiAddress + "/Users/show/" + m_nUserId + "?access_token=" + m_strAccessToken;
        var strResponse = g_DreamCraft.httpGet(url);
        if( !strResponse ) return;
        var jsonResponse = JSON.parse(strResponse);
        if( jsonResponse.hasOwnProperty("uid") ) {
            m_strUsername = jsonResponse.username;
            m_strNoavatar = jsonResponse.noavatar;
        }
    }
}
