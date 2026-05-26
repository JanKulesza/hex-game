import QtQuick
import QtQuick.Controls
import "widgets"
import App

Item {
	id: menuScreen
	anchors.fill: parent
	Column {
		anchors.centerIn: parent
		spacing: 0
		Column {
			anchors.horizontalCenter: parent.horizontalCenter
			Text {
				text: "HEX"
				font.pixelSize: 82
				font.letterSpacing: 6
				font.weight: 600
				color: "white"
				anchors.horizontalCenter: parent.horizontalCenter
			}
			Text {
				text: "Play with AI"
				font.pixelSize: 16
				font.letterSpacing: 4
				color: "#1654b7"
				anchors.horizontalCenter: parent.horizontalCenter
			}
		}
		Image {
			source: "qrc:/resources/menuImage.png"
			height: 250
			width:375
		}

		Column {
			anchors.horizontalCenter: parent.horizontalCenter
			spacing: 12
			PrimaryBtn {
				primary: true
				icon: "🎲"
				text: "New Game"
				onClicked: {
					stack.push(Qt.resolvedUrl("NewGameScreen.qml"))
					Board.resetGame();
				}
			}
			PrimaryBtn {
			    icon: "⚙"
			    text: "Settings"
			}
			PrimaryBtn {
			    icon: "📊"
			    text: "Statistics"
			}
			
			PrimaryBtn {
			    icon: "ℹ️"
			    text: "About"
			}
			
			PrimaryBtn {
			    icon: "🚪"
			    text: "Exit"
			    onClicked: Qt.quit()
			}
		}
	}
}
