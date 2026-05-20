import QtQuick
import QtQuick.Controls

Item {
    id: secondaryBtn

    property string text: ""
    property string secondaryText: ""
    property string icon: ""
    property double fontSize: 1.0
    property int w: 300
    property int h: 56 
    property bool active: false

    signal clicked()

    width: secondaryBtn.w
    height: secondaryBtn.h

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 10
        color: {
            if(mouseArea.containsMouse || secondaryBtn.active)
                return "#401654b7"
            return "#09131d"
        } 
        border.color: {
            if(mouseArea.containsMouse || secondaryBtn.active)
                return "#1654b7"
            return "#19222b"
        } 
        border.width: 1

        Behavior on color {
            ColorAnimation { duration: 120 }
        }

        Row {
            anchors.margins: 12
            anchors.centerIn: secondaryBtn.secondaryText == "" ? parent : null
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: secondaryBtn.secondaryText == "" ? null : parent.left
            spacing: 12

            Text {
                text: secondaryBtn.icon
                font.pixelSize: 16
                opacity: 1
                visible: secondaryBtn.icon !== ""
                anchors.verticalCenter: parent.verticalCenter
            }
            Column { 
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Text {
                    text: secondaryBtn.text
                    font.pixelSize: 13 * secondaryBtn.fontSize
                    font.letterSpacing: 2
                    font.weight: 600
                    color: "white"
                }
                Text {
                    text: secondaryBtn.secondaryText 
                    font.pixelSize: 10 * secondaryBtn.fontSize
                    font.letterSpacing: 2
                    color: "#80ffffff"
                }
            }
        }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: secondaryBtn.clicked()
    }
}