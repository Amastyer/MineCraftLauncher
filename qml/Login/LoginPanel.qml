import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import "../MyCom"

Item {
    id: root
    width: 366
    height: 514
    signal logined

    Image { anchors.fill: parent; source: "../images/login_bg.png" } // 背景图片

    Item {
        anchors.fill: parent

        Row {  // 隐藏和退出按钮
            spacing: 10
            anchors.topMargin: 20
            anchors.rightMargin: 15
            anchors.top: parent.top
            anchors.right: parent.right

            Button { width: 16; height: 16; onClicked: showMinimized(); style: ButtonStyle { background: Image { source: "../images/login_min_btn.png" } } }
            Button { width: 16; height: 16; onClicked: Qt.quit(); style: ButtonStyle { background: Image { source: "../images/login_close_btn.png" } } }
        }

        Column {
            id: loginLayout
            width: 310
            anchors.topMargin: 50
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            Image { source: "../images/login_logo_03.png"; anchors.horizontalCenter: parent.horizontalCenter } // logo图片

            Item {
                width: 310; height: 56
                TextField { // 邮箱输入框
                    id: usernameInput
                    width: 310
                    height: 36
                    style: textFieldStyle
                    text: m_bRememberPassword ? m_strEmail : ""
                    anchors.bottom: parent.bottom
                    validator: RegExpValidator {
                        regExp: /[0-9a-zA-Z`~\!@\#\$%\^&\*\(\)\-\=_\+\[\]\{\}\\\|;\:'"<>\?,\.\/]{6,32}/
                    }
                }
            }

            Item {
                width: 310; height: 46
                TextField {
                    id: passwordInput
                    width: 310; height: 36
                    style: textFieldStyle
                    text: m_bRememberPassword ? m_strPassword : ""
                    anchors.bottom: parent.bottom
                    echoMode: TextInput.Password
                }
            }

            Item {
                width: 310; height: 29
                CheckBox {
                    id: rpCheckBox
                    height: 19
                    text: "记住密码"
                    style: checkBoxStyle
                    checked: m_bRememberPassword
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                }

                CheckBox {
                    height: 19
                    text: "自动登录"
                    checked: m_bAutoLogin
                    style: checkBoxStyle
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    onCheckedChanged: {
                        rpCheckBox.checked = true
                        g_DcSettings.setAutoLogin(checked)
                    }
                }
            }

            Item {
                width: 310; height: 51
                DcButton {
                    width: 156; height: 41
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    source: Qt.resolvedUrl("../images/login_default-s.png")
                    onClicked: login(usernameInput.text, passwordInput.text)
                }

                DcButton { // 无法登录按钮
                    width: 148; height: 41
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    source: Qt.resolvedUrl("../images/Nlogin_default-s.png")
                    onClicked: Qt.openUrlExternally("http://www.i5mc.com/member.php?mod=logging&action=login&viewlostpw")
                }
            }

            Item { height: 10; width: 310 }
            Rectangle {
                id: messageBox
                width: 366
                height: 0
                visible: true
                color: "green"
                anchors.horizontalCenter: parent.horizontalCenter
                property string text: ""
                onHeightChanged: messageInfo.visible = height == 0 ? false : true
                Text { id: messageInfo; text: parent.text; color: "white"; anchors.centerIn: parent }
            }
            Item { width: 310; height: 10 }

            DcButton { // QQ登录按钮
                width: 310; height: 46
                source: Qt.resolvedUrl("../images/QQ_default_s.png")
            }

            Item { width: 310; height: 5 }

            DcButton { // 注册按钮
                width: 310; height: 46
                source: Qt.resolvedUrl("../images/enrol_default-s.png")
                onClicked: Qt.openUrlExternally("http://www.i5mc.com/member.php?mod=register")
            }
        }
    }

    // +---------------------------------------------------
    // |                自定义控件风格
    // +---------------------------------------------------
    Component { // 文本输入框Style
        id: textFieldStyle
        TextFieldStyle {
            background: Item {
                clip: true
                Image {
                    width: control.width
                    height: control.height * 3
                    source: "../images/mima.png"
                    y: control.focus ? (0 - height / 3 * 2) : ( control.hovered ? (0 - height / 3) : 0 )
                }
            }
        }
    }

    Component {
        id: checkBoxStyle
        CheckBoxStyle {
            indicator: Item {
                width: height
                height: control.height
                clip: true
                Image {
                    width: parent.width
                    height: parent.height * 3
                    source: "../images/status_default-s.png"
                    y: control.checked ? (0 - height / 3 * 2) : ( control.hovered ? (0 - height / 3) : 0 )
                }
            }
            label: Text {
                text: control.text
                color: "white"

            }
        }
    }

    PropertyAnimation {
        id: messageBoxIn
        target: messageBox
        property: "height"
        from: 0
        to: 30
        duration: 400
    }

    PropertyAnimation {
        id: messageBoxOut
        target: messageBox
        property: "height"
        from: 30
        to: 0
        duration: 400
    }

    function login( email, password ) {
        if( rpCheckBox.checked ) {
            g_DcSettings.setEmail(usernameInput.text);
            g_DcSettings.setPassword(passwordInput.text);
            g_DcSettings.setRememberPassword( true );
        }
        else g_DcSettings.setRememberPassword( false )

        loginLayout.enabled = false
        messageBox.color = "green";
        messageBox.text = "登录中";
        messageBoxIn.start();
        var query = {};
        var url = m_strApiAddress + "/users/login";
        if( checkEmail( email ) ) {
            query = { email: email, password: password };
        } else {
            query = { username: email, password: password };
        }
        console.log( JSON.stringify( query ) );
        var data = g_DreamCraft.httpPost( url, JSON.stringify( query ), 5000 );
        console.log(data);
        if( data ) data = JSON.parse(data);
        if( data.hasOwnProperty("error") ) {
            loginError( data.error.status )
        } else if( data.hasOwnProperty("created") ) {
            m_strAccessToken = data.id
            m_nUserId = data.userId
            loginAccess()
        } else {
            loginError( 0 )
        }
    }

    function loginError( code ) {
        switch( code ) {
        case 401:
            messageBox.color = "red";
            messageBox.text = "邮箱或密码错误！";
            messageBoxIn.start();
            break;
        default:
            messageBox.color = "red";
            messageBox.text = "登录失败，请检查网络";
            messageBoxIn.start();
            break;
        }
        loginLayout.enabled = true
    }

    function loginAccess() {
        messageBoxIn.stop()
        messageBoxOut.start()
        loginLayout.enabled = true
        logined()
    }

    function checkEmail( email ) {
         var pattern = /^([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+(.[a-zA-Z0-9_-])+/;
         if( pattern.test( email ) ) return true;
         return false;
    }
}
