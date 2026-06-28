import QtQuick
import QtQuick.Shapes
import "widgets"
import App

Item {
   
    anchors.fill: parent
    Column {
        spacing: 16
        anchors.margins: 16
        anchors.fill: parent
        Text {
            text: "New Game Settings"
            color: "white"
            font.pixelSize: 28
            anchors.horizontalCenter: parent.horizontalCenter
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
                        active: Board.size == modelData
                        onClicked: Board.size = modelData
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
                    active: Board.playersColor == Game.Color.Blue
                    onClicked: Board.playersColor = Game.Color.Blue
                }
                SecondaryBtn {
                icon: "🔴"
                    text: "RED"
                    fontSize: 1.1
                    w: parent.width * 0.5 - 8
                    h: 70
                    secondaryText: "north-south"
                    active: Board.playersColor == Game.Color.Red
                    onClicked: Board.playersColor = Game.Color.Red
                }
            }
        }
        CardSection {
            label: "AI DIFFICULTY LEVEL"
            Row {
                spacing: 16
                width: parent.width
                Repeater {
                    model: [{label:"EASY", d: Game.Difficulty.Easy},{label:"MEDIUM", d: Game.Difficulty.Medium},{label:"HARD", d: Game.Difficulty.Hard},{label:"EXPERT", d: Game.Difficulty.Expert}]
                    SecondaryBtn {
                        text: modelData.label
                        height: 40
                        width: parent.width / 4 - 3*16/4
                        active: Board.aiDifficulty == modelData.d
                        onClicked: Board.aiDifficulty = modelData.d
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
                    icon: "🔵"
                    text: "BLUE"
                    // - 8, spacing 16px/2
                    w: parent.width * 0.5 - 16 / 2
                    h: 54
                    secondaryText: Board.playersColor == Game.Color.Blue ? "Player" : "AI"
                    active: Board.whoStarts == Game.Color.Blue
                    onClicked: Board.whoStarts = Game.Color.Blue
                }
                SecondaryBtn {
                    icon: "🔴"
                    text: "RED"
                    w: parent.width * 0.5 - 8
                    h: 54
                    secondaryText: Board.playersColor == Game.Color.Blue ? "AI" : "Player"
                    active: Board.whoStarts == Game.Color.Red
                    onClicked: Board.whoStarts = Game.Color.Red
                }
            }
        }
        PrimaryBtn {
            primary: true
            icon: "🎲"
            text: "START GAME"
            width: parent.width
            onClicked: {
                Board.createBoard()
                stack.push(Qt.resolvedUrl("GameScreen.qml"))
            }
        }
        SecondaryBtn {
            text: "Back"
            h: 56
            width: parent.width
            onClicked: stack.pop()
        }
    }
}