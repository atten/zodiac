import QtQuick 1.0

Text {
    property string helpTip

    objectName: "label"
    width: 192
    smooth: true
    color: "#a5afcf"
    font.family: "DejaVu Sans Condensed"
    font.pointSize: 10
    horizontalAlignment: Text.AlignHCenter

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            view.requestHelp(helpTip)
        }
    }
}

