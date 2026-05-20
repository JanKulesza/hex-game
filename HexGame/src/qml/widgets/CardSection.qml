import QtQuick

Item {
    property string label: ""
    default property alias cardContent: container.children

    height: mainLayout.height + 32
    width: parent.width

	Rectangle {
            color: "#09131d"
            radius: 10
            border.color: "#19222b"
            anchors.fill: parent

            Column {
                id: mainLayout
                height: childrenRect.height
                width: parent.width - 32
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 16
                spacing: 10

                Text {
                    text: label
                    color: "white"
                    font.weight: 700
                    font.pixelSize: 14
                    font.letterSpacing: 4
                }
                Column {
                    id: container
                    width: parent.width
                    spacing: 10
                    height: childrenRect.height
                }
            }
        }
}