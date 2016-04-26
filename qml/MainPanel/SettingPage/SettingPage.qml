import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import DcSettings 0.1
import "../../MyCom"

Item {
    id: root
    width: 974
    height: 691
    property int intMemory: 0
    property bool isAutoStart
    property bool isAutoLogin
    property bool isAutoUpdate
    property string strJavaPath: ""
    property var listMemory: []

    Image { anchors.fill: parent; source: "../../images/setting_bg.png" }

    Column {
        width: root.width * 0.92
        anchors.horizontalCenter: parent.horizontalCenter

        /* 顶部标题 */
        Item { width: parent.width; height: 50 }
        Text { text: "启动器设置"; color: "white"; font.pixelSize: 30; font.bold: true; font.family: "Microsoft YaHei UI"; anchors.horizontalCenter: parent.horizontalCenter }

        /* 开机自启 */
        Item { width: parent.width; height: 80 }
        CheckBox { id: autoStartCheckBox; text: "开机自启"; height: 25; style: checkBoxStyle }
        Item { width: parent.width; height: 5 }
        Text { text: "为了更好的支持我们，请您保持开机自动启动哟~"; color: "white"; font.pixelSize: 18; font.family: "微软雅黑" }

        /* 自动登陆 */
        Item { width: parent.width; height: 15 }
        CheckBox { id: autoLoginCheckBox; text: "自动登录"; height: 25; style: checkBoxStyle }
        Item { width: parent.width; height: 5 }
        Text { text: "打开启动器，将为您自动登录，帮您节约启动时间。"; color: "white"; font.pixelSize: 18; font.family: "微软雅黑" }

        /* 自动更新 */
        Item { width: parent.width; height: 15 }
        CheckBox { id: autoUpdateCheckBox; text: "自动更新"; height: 25; style: checkBoxStyle }
        Item { width: parent.width; height: 5 }
        Text { text: "选择自动更新，以确保您可以收到最新的游戏通知。"; color: "white"; font.pixelSize: 18; font.family: "微软雅黑" }

        /* Java内存和路径 */
        Item { width: parent.width; height: 15 }
        Item {
            width: parent.width; height: 77
            Item {
                width: parent.width - autoSetBtn.width - 10; height: parent.height
                Row { // Java内存
                    spacing: 10
                    width: parent.width
                    anchors.top: parent.top
                    Text { id: javaMemoryText;width: javaPathText.width; text: "内存设置:"; color: "white"; font.pixelSize: 25; font.family: "方正特粗光辉简体"; anchors.verticalCenter: parent.verticalCenter }
                    ComboBox { id: javaMemoryInput; height: 33; width: parent.width - javaMemoryText.width - systemMemoryText.width - parent.spacing * 2; model: listMemory; editable: true
                        onEditTextChanged: { if( editText > intMemory ) editText = editText.substring( 0, editText.length - 1 ) }
                    }
                    Text { id: systemMemoryText; text: "系统物理内存 " + intMemory + " MB"; color: "#C5C5C5"; font.pixelSize: 22; font.family: "方正特粗光辉简体"; anchors.bottom: parent.bottom }
                }

                Row { // Java路径
                    spacing: 10
                    width: parent.width
                    anchors.bottom: parent.bottom
                    Text { id: javaPathText;text: "JAVA路径:"; color: "white"; font.pixelSize: 25; font.family: "方正特粗光辉简体"; anchors.verticalCenter: parent.verticalCenter }
                    TextField { id: javaPathInput; height: 33; font.family: "微软雅黑"; width: parent.width - javaPathText.width - fileBtn.width - parent.spacing * 2 }
                    DcButton {
                        id: fileBtn
                        width: 79; height: 33
                        source: Qt.resolvedUrl("../../images/open_file.png")
                        onClicked: fileDialog.visible = true
                        FileDialog {
                            id: fileDialog
                            title: "选择Java可执行程序路径"
                             nameFilters: Qt.platform.os === "windows" ? [ "java.exe javaw.exe" ] : ["java"]
                             onAccepted: {
                                 var str = fileDialog.fileUrl.toString();
                                 javaPathInput.text = str.replace("file:///", "")
                             }

                             onRejected: console.log( "Canceled" )
                        }
                    }
                }
            }
            DcButton { id: autoSetBtn; width: 159; height: 77; source: Qt.resolvedUrl("../../images/autosetting_btn.png"); anchors.right: parent.right; onClicked: autoSettings() }
        }

        Item { width: parent.width; height: 40 }
        Row {
            spacing: 100
            anchors.horizontalCenter: parent.horizontalCenter
            DcButton { id: cancelBtn; width: 147; height: 41; source: Qt.resolvedUrl("../../images/cancel_btn.png"); onClicked: root.visible = false  }
            DcButton { id: saveBtn; width: 156; height: 41; source: Qt.resolvedUrl("../../images/save_btn.png"); onClicked: saveSettings() }
        }
    }

    Component.onCompleted: {
        autoStartCheckBox.checked = g_DcSettings.autoStart
        autoLoginCheckBox.checked = g_DcSettings.autoLogin
        autoUpdateCheckBox.checked = g_DcSettings.autoUpdate
        javaPathInput.text = g_DcSettings.javaPath
        intMemory = g_DcSettings.getSystemMemory()
        getMemoryList();
    }

    function saveSettings() {
        g_DcSettings.setAutoStart( autoStartCheckBox.checked )
        g_DcSettings.setAutoLogin( autoLoginCheckBox.checked )
        g_DcSettings.setAutoUpdate( autoUpdateCheckBox.checked )
        g_DcSettings.setMaxMem( javaMemoryInput.editText || javaMemoryInput.currentText )
        g_DcSettings.setJavaPath( javaPathInput.text )
        root.visible = false
    }

    function getMemoryList( num ) {
        if( g_DcSettings.is64BitSystem() && g_DcSettings.is64BitJava()  ) {
            var nMemory = g_DcSettings.getSystemMemory();
            var arr = [];
            for( var i = 512; i < nMemory; i++ ) {
                if( ( i === 512 || i % 1024 === 0 ) && i <= 4096 ) {
                    console.log( i );
                    arr.push( i );
                }
            }
            listMemory = arr;
        } else {
            listMemory = [ 512, 1024 ];
        }

        for( var i = 512; i < num; i++ ) {
            if( i % 512 == 0 ) arr.push( i );
        }
        return arr;
    }

    function autoSettings() {
        getMemoryList();
        var path = g_DcSettings.getSystemJavaPath();
        if( !path ) return;
        javaPathInput.text = path
    }

    Component {
        id: checkBoxStyle
        CheckBoxStyle {
            indicator: Row {
                spacing: 10
                Text {  text:control.text; color: "white"; font.pixelSize: control.height; font.family: "方正特粗光辉简体"; }
                Item {
                    width: height
                    height: control.height
                    clip: true
                    Image {
                        width: parent.width
                        height: parent.height * 3
                        source: "../../images/status_default-s.png"
                        y: control.checked ? (0 - height / 3 * 2) : ( control.hovered ? (0 - height / 3) : 0 )
                    }
                }
            }
            label: Text {}
        }
    }
}
