import QtQuick
import QtQuick.Controls

Item {
    id: primaryBtn

    property string text: ""
    property string icon: ""
    property bool primary: false

    signal clicked()

    width: 300
    height: 56

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 10
        color: {
            if(mouseArea.containsMouse)
                return primary ? "#FF1654b7" : "#401654b7"
            return primary ? "#CC1654b7" : "#001654b7"
        } 
        border.color: {
            if(mouseArea.containsMouse)
                return "#1654b7"
            return primary ? "#CC1654b7" : "#19222b"
        } 
        border.width: 1

        Behavior on color {
            ColorAnimation { duration: 120 }
        }

        Row {
            anchors.centerIn: parent
            spacing: 12

            Text {
                text: primaryBtn.icon
                font.pixelSize: 20
                opacity: {
                    if(mouseArea.containsMouse)
                        return 1
                    return primary ? 1 : 0.9
                } 
                visible: primaryBtn.icon !== ""
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: primaryBtn.text
                font.pixelSize: 13
                font.letterSpacing: 2
                font.bold: true
                color: {
                    if(mouseArea.containsMouse)
                        return "white"
                    return primary ? "white" : "#80ffffff"
                } 
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: primaryBtn.clicked()
    }
}