import QtQuick 2.0
import "../MyCom"
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    width: 1280
    height: 71

    signal logoClicked
    signal minBtnClicked
    signal closeBtnClicked
    signal setBtnClicked

    Image { anchors.fill: parent; source: "../images/header_bg.png" }

    Image {
        anchors.topMargin: 8
        anchors.leftMargin: 65
        anchors.top: parent.top
        anchors.left: parent.left
        source: "../images/logo_x.png"
        MouseArea {
            anchors.fill: parent
            onClicked: logoClicked()
        }
    }

    Row {
        spacing: 15
        anchors.topMargin: 10
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.right: parent.right

        DcButton {
            width: 15
            height: 15
            source: Qt.resolvedUrl("../images/cart_default_s.png")
            onClicked: Qt.openUrlExternally("http://shop.i5mc.com")
        }

        DcButton {
            width: 15
            height: 15
            source: Qt.resolvedUrl("../images/shezhi_default_s.png")
            onClicked: contentMenu.popup()

            Menu {
                id: contentMenu
                MenuItem { text: "注销"; onTriggered: logout() }
                MenuItem { text: "设置"; onTriggered: setBtnClicked() }
            }
        }

        Button {
            width: 10
            height: 10
            onClicked: minBtnClicked()
            style: ButtonStyle {
                background: Item {
                    clip: true
                    Image {
                        width: control.width
                        height: control.height * 2
                        source: "../images/window1.png"
                        y: control.hovered ? 0 - height / 2 : 0
                    }
                }
            }
        }

        Button {
            width: 10
            height: 10
            onClicked: closeBtnClicked()
            style: ButtonStyle {
                background: Item {
                    clip: true
                    Image {
                        width: control.width
                        height: control.height * 2
                        source: "../images/window3.png"
                        y: control.hovered ? 0 - height / 2 : 0
                    }
                }
            }
        }
    }
}
