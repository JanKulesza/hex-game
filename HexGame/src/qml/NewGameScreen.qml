import QtQuick
import QtQuick.Shapes
import "widgets"

Item {
    property int boardSize: 9
    property string playersColor: "BLUE"
    property string aiDifficulty: "MEDIUM"
    property string whoStarts: "PLAYER"

    anchors.fill: parent
    Column {
        spacing: 16
        anchors.margins: 16
        anchors.fill: parent
        Item {
            width: parent.width
            height: 48

            SecondaryBtn {
                icon: "⬅️"
                text: "Back"
                w: 96
                h: 48
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                onClicked: stack.pop()
            }

            Text {
                text: "New Game Settings"
                color: "white"
                font.pixelSize: 28
                anchors.centerIn: parent    
            }
        }

        CardSection {
            label: "BOARD SIZE"
            Row {
                spacing: 16
                width: parent.width
                Repeater {
                    model: [5, 7, 8, 9, 11, 13]
                    SecondaryBtn {
                        text: modelData + "x" + modelData
                        height: 40
                        width: parent.width / 6 - 5*16/6
                        active: boardSize === modelData
                        onClicked: boardSize = modelData
                    }
                }
            }
        }
        CardSection {
            label: "PLAYER's COLOR"
            Row {
                width: parent.width
                spacing: 16 
                SecondaryBtn {
                    icon: "🔵"
                    text: "BLUE"
                    fontSize: 1.1
                    // - 8, spacing 16px/2
                    w: parent.width * 0.5 - 16 / 2
                    h: 70
                    secondaryText: "west-east"
                    active: playersColor == "BLUE"
                    onClicked: playersColor = "BLUE"
                }
                SecondaryBtn {
                icon: "🔴"
                    text: "RED"
                    fontSize: 1.1
                    w: parent.width * 0.5 - 8
                    h: 70
                    secondaryText: "north-south"
                    active: playersColor == "RED"
                    onClicked: playersColor = "RED"
                }
            }
        }
        CardSection {
            label: "AI DIFFICULTY LEVEL"
            Row {
                spacing: 16
                width: parent.width
                Repeater {
                    model: ["EASY","MEDIUM","HARD","EXPERT"]
                    SecondaryBtn {
                        text: modelData
                        height: 40
                        width: parent.width / 4 - 3*16/4
                        active: aiDifficulty === modelData
                        onClicked: aiDifficulty = modelData
                    }
                }
            }
        }
        CardSection {
            label: "WHO STARTS?"
            Row {
                width: parent.width
                spacing: 16 
                SecondaryBtn {
                    icon: playersColor == "BLUE" ? "🔵" : "🔴"
                    text: "PLAYER"
                    // - 8, spacing 16px/2
                    w: parent.width * 0.5 - 16 / 2
                    h: 54
                    secondaryText: playersColor == "BLUE" ? "Blue" : "Red"
                    active: whoStarts == "PLAYER"
                    onClicked: whoStarts = "PLAYER"
                }
                SecondaryBtn {
                    icon: playersColor == "BLUE" ? "🔴" : "🔵"
                    text: "AI"
                    w: parent.width * 0.5 - 8
                    h: 54
                    secondaryText: playersColor == "BLUE" ? "Red" : "Blue"
                    active: whoStarts == "AI"
                    onClicked: whoStarts = "AI"
                }
            }
        }
    }
}