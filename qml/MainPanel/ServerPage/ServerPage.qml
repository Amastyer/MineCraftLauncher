import QtQuick 2.3
import "../../MyCom"

Item {
    width: 974
    height: 649
    signal select( string name )

    property var serverListArr: m_listServerArr

    onServerListArrChanged: {
        listModel.clear();
        for( var i = 0; i < serverListArr.length; i++ ) {
            listModel.append(serverListArr[i]);
        }
    }

    ListModel { id: listModel }

// +------------------------------------------------------------
// |                     服务器信息控件
// +------------------------------------------------------------
    Component {
        id: component

        Item {
            id: serverItem
            width: 275
            height: 524
            focus: false

            property bool hovered
            property string remoteAddress: address
            property string localDirPath: path
            property string serverName: name
            property string clientStatus: status

            // 背景图片
            Image {
                id: versionBg
                anchors.fill: parent
//                source: Qt.resolvedUrl("../../images/banben_bg.png")
                source: parent.focus ? "../../images/banben_bg_hover.png" : (hovered ? "../../images/banben_bg_hover.png" : "../../images/banben_bg.png")
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onHoveredChanged: hovered = hovered ? false : true

                    onClicked: {
                        serverItem.focus = true
                        select( name )
                    }
                }
            }

            Item {
                anchors.centerIn: parent
                width: 250
                height: 500

                Image {
                    width: 250
                    height: 440
                    source: typeof image === "undefined" ? Qt.resolvedUrl("../../images/banb_img.png") : image

                    Image {
                        anchors.centerIn: parent
                        visible: clientStatus === "finished" ? false : true
                        source: clientStatus === "empty" ? "../../images/xiaz.png" : "../../images/xiaz_ing.png"
                    }
                }

                Image {
                    anchors.bottom: parent.bottom
                    source: parent.parent.focus || hovered ? "../../images/banben_botDJ.png" : "../../images/banben_botMR.png"
                    Row {
                        spacing: 10
                        anchors.centerIn: parent
                        Image {
                            anchors.verticalCenter: parent.verticalCenter;
                            source: clientStatus === "finished" ? Qt.resolvedUrl("../../images/xz.png") : Qt.resolvedUrl("../../images/xzz.png")

                        }
                        Text { anchors.verticalCenter: parent.verticalCenter; text: name; color: "white"; font.pixelSize: 24 }
                    }
                }
            }
        }
    }

    Item {
        anchors.centerIn: parent
        width: 925
        height: 524

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            width: 42
            height: 68
            color: "#000"
            opacity: 0.5
            Image { anchors.centerIn: parent; source: "../../images/jiantou.png" }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if( list.x >= 0 ) return;
                    list.x += 275;
                    enabled = false;
                    leftTimer.start();
                }

                Timer { id: leftTimer; interval: 500; onTriggered: parent.enabled = true }
            }
        }

        Item {
            anchors.centerIn: parent

            width: 275 * 3
            height: 524
            clip: true

            Row {
                id: list
                height: parent.height

                Repeater {
                    model: listModel
                    delegate: component
                }

                Behavior on x { PropertyAnimation { duration: 500 } }
            }
        }

        Rectangle {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            width: 42
            height: 68
            color: "#000"
            opacity: 0.5
            Image { anchors.centerIn: parent; source: "../../images/jiantouR.png" }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if( list.x <= 0 - ( list.width - list.parent.width ) ) return;
                    list.x -= 275;
                    enabled = false;
                    rightTimer.start()
                }

                Timer { id: rightTimer; interval: 500; onTriggered: parent.enabled = true }
            }
        }
    }

    Component.onCompleted: getServerInfo()

    function getServerInfo() {
        var jsonResponse;
        try {
            jsonResponse = JSON.parse(g_DreamCraft.httpGet("http://play.i5mc.com:10010/serverlist.json", 10000))
        } catch(e) {
            return;
        }
        m_listServerArr = jsonResponse
    }
}
