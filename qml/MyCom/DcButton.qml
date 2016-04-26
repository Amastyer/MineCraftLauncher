import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button {
    property string source: ""
    style: ButtonStyle {
        panel: Item {
            clip: true
            Image {
                source: control.source
                width: control.width
                height: control.height * 3
                y: control.pressed ? (0 - height / 3 * 2) : ( control.hovered ? (0 - height / 3) : 0 )
            }
        }
    }
}
