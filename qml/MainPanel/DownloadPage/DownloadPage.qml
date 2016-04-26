import QtQuick 2.0

Item {
    width: 974
    height: 649

    property string currentText: ""
    property int currentIndex: 0
    property var serverListArr: m_listServerArr

    onServerListArrChanged: {
        listModel.clear();
        for( var i = 0; i < serverListArr.length; i++ ) {
            console.log( serverListArr[i].status )
            listModel.append(serverListArr[i]);
        }
    }

    ListModel { id: listModel }

    Component {
        id: component

        DownloadPanel {
            serverName: name
            remoteAddress: address
            localDirPath: path
            clientStatus: status
            strMd5: md5sum
            objNotice: notice
            objInform: inform
            strClientVersion: version
            anchors.fill: parent
        }
    }

    Repeater {
        id: repeater
        model: listModel
        delegate: component
    }

    function select( name ) {
        for( var i = 0, max = repeater.count; i < max; i++ ) {
            if( repeater.itemAt(i).serverName === name ) {
                repeater.itemAt(i).visible = true;
            } else {
                repeater.itemAt(i).visible = false;
            }
        }
    }
}

