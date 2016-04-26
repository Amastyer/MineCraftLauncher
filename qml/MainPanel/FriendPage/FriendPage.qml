import QtQuick 2.0
import "../../MyCom"
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    width: 306
    height: 649

    Image { anchors.fill: parent; source: "../../images/haoY.png" }

    Column {
        width: parent.width *  0.93
        height: parent.height - parent.width * 0.07
        anchors.centerIn: parent

        Item {
            width: parent.width
            height: 82
            Image { id: noavatar; width: 82; height: 82; anchors.left: parent.left; source: m_strNoavatar }
            Item { width: 20; height: parent.height }
            Item {
                anchors.leftMargin: 10
                width: parent.width - noavatar.width - 20
                height: parent.height
                anchors.left: noavatar.right
                Text { text: m_strUsername; color: "white"; font.pixelSize: 24; font.family: "微软雅黑" }
                Image { source: "../../images/touch_tag.png"; anchors.bottom: parent.bottom; anchors.right: parent.right
                    Text { anchors.centerIn: parent; text: "您还未创建角色" }
                }
            }
        }

        Item { width: parent.width; height: 8 }
        Image { width: parent.width; height: 30; source: "../../images/tag_bg.png"
            Row {
                spacing: ( width - 92 * 3 ) / 2
                anchors.fill: parent
                DcButton { width: 92; height: 30; source: Qt.resolvedUrl( "../../images/tag_btn.png" )
                    Text { anchors.centerIn: parent; text: "个人信息" }
                }
                DcButton { width: 92; height: 30; source: Qt.resolvedUrl( "../../images/tag_btn.png" )
                    Text { anchors.centerIn: parent; text: "好友列表" }
                }
                DcButton { width: 92; height: 30; source: Qt.resolvedUrl( "../../images/tag_btn.png" )
                    Text { anchors.centerIn: parent; text: "皮肤管理" }
                }
            }
        }

        Item { width: parent.width; height: 8 }
        Image { source: "../../images/friend_bg.png"; width: parent.width; height: 500
            TextField {
                anchors.top: parent.top
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.9
                height: 30
                style: TextFieldStyle {
                    background: Rectangle {
                        radius: 6
                        color: "white"
                    }
                }
                Image { source: "../../images/search_btn.png"; anchors.right: parent.right; anchors.rightMargin: 10; anchors.verticalCenter: parent.verticalCenter
                    MouseArea { anchors.fill: parent }
                }
            }
        }
    }
}
