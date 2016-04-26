import QtQuick 2.3
import "../../MyCom"
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import DownloadProcess 0.1

Item {
    id: root
    width: 974
    height: 649

    property string remoteAddress: ""
    property string localDirPath: ""
    property string serverName: ""
    property string clientStatus: ""
    property string strMd5: ""
    property string strClientVersion: ""
    property var objNotice
    property var objInform
    property int nCurrent: downloadProcess.current
    property int nTotal: downloadProcess.total
    property string strMeaasge: downloadProcess.message
    onNCurrentChanged: {
        if( nTotal < 1 && nCurrent > nTotal ) return;
        progressBar.value = nCurrent;
        progressBar.maximumValue = nTotal;
        textProgress.text = Math.round( nCurrent / nTotal * 10000 ) / 100.00 + "%"
    }

    Item {
        width: 974
        height: 520

        Row {
            spacing: 20
            anchors.leftMargin: 55
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            Image { width: 602; height: 482; source: "../../images/fwqL_bg.png"
                Rectangle { width: parent.width; height: noticeColumn.height; color: Qt.rgba( 0, 0, 0, 0.6 )
                    anchors.bottom: parent.bottom
                    Column {
                        id: noticeColumn
                        width: parent.width * 0.96
                        anchors.horizontalCenter: parent.horizontalCenter
                        Item { width: parent.width; height: 20 }
                        Text { text: objNotice.title || ""; font.pixelSize: 22; color: "white" }
                        Item { width: parent.width; height: 10 }
                        Text { text: objNotice.content || ""; font.pixelSize: 16; color: "white"; width: parent.width; wrapMode: Text.WordWrap; lineHeight: 1.5 }
                        Item { width: parent.width; height: 20 }
                    }
                }
            }

            Image {
                width: 282
                height: 482
                source: "../../images/fwqR_bg.png"
                Rectangle { width: parent.width; height: informColumn.height; color: Qt.rgba( 0, 0, 0, 0.6 )
                    anchors.bottom: parent.bottom
                    Column {
                        id: informColumn
                        width: parent.width * 0.9
                        anchors.horizontalCenter: parent.horizontalCenter
                        Item { width: parent.width; height: 20 }
                        Text { text: objInform.title || {}; font.pixelSize: 22; color: "white" }
                        Item { width: parent.width; height: 10 }
                        Text { text: objInform.content || ""; font.pixelSize: 16; color: "white"; width: parent.width; wrapMode: Text.WordWrap; lineHeight: 1.5 }
                        Item { width: parent.width; height: 20 }
                    }
                }
            }
        }
    }

    Image {
        anchors.bottom: parent.bottom

        width: 974
        height: 129
        source: "../../images/foot_bg.png"

        Item {
            width: 500
            height: 86
            visible: downloadProcess.status === "finished" ? false : true

            anchors.left: parent.left
            anchors.leftMargin: 55
            anchors.verticalCenter: parent.verticalCenter

            ProgressBar {
                id: progressBar
                width: 364
                height: 86
                value: 0
                maximumValue: 1
                orientation: Qt.Horizontal
                style: ProgressBarStyle {
                    background: Image {
                        source: "../../images/jind.png"
                    }
                    progress: Image {
                        source: "../../images/jind_hover.png"
                    }
                }

                onValueChanged: Math.floor(progressBar.value / progressBar.maximumValue) * 100 + "%"

                Text {
                    anchors.centerIn: parent
                    text: serverName
                    color: "white"
                    font.pixelSize: 50
                }
            }

            Item {
                anchors.leftMargin: 10
                anchors.left: progressBar.right
                height: parent.height

                Text {
                    id: textProgress
                    color: "white"
                    text: "0%"
                    font.pixelSize: 45
                    anchors.top: parent.top
                }

                Text {
                    id: downloadMeassge
                    color: "white"
                    font.pixelSize: 25
                    text: strMeaasge
                    anchors.bottom: parent.bottom
                }
            }
        }

        DcButton {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 15
            width: 223
            height: 82
            source: Qt.resolvedUrl("../../images/Play.png")
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if( downloadProcess.status === "finished" ) {
                        g_DreamCraft.startGame( m_strUsername, m_nMaxMem, g_DcSettings.javaPath, strClientVersion, localDirPath, m_nUserId, m_strAccessToken );
                    } else {
                        downloadProcess.startDownload();
                    }
                }
            }
        }
    }

    DownloadProcess {
        id: downloadProcess
        localPath: localDirPath
        remoteAddress: root.remoteAddress
        Component.onCompleted: {
            checkClientStatus( strMd5 );
        }

        onStatusChanged: {
            for( var i = 0, max = m_listServerArr.length; i < max; i++ ) {
                if( m_listServerArr[i].name === serverName ) {
                    m_listServerArr[i].status = downloadProcess.status;
                }
            }
        }
    }
}
