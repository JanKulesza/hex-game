import QtQuick
import QtQuick.Layouts
import "widgets"
import App

Item {
    Component.onCompleted: {
    }
	anchors.fill: parent
	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 16
		Rectangle {
		    color: "#09131d"
		    radius: 10
		    border.color: "#19222b"
			Layout.fillWidth: true
			height: childrenRect.height + 32
			RowLayout {
				anchors.verticalCenter: parent.verticalCenter
				anchors.margins: 16
				anchors.left:parent.left
				width: parent.width - 32
				height: 56
				Row {
					anchors.verticalCenter: parent.verticalCenter
					spacing: 12
					Text {
					    text: "🔵"
					    font.pixelSize: 16
					    anchors.verticalCenter: parent.verticalCenter
					}
					Column { 
					    anchors.verticalCenter: parent.verticalCenter
					    spacing: 2
					    Text {
					        text: "You (Blue)"
					        font.pixelSize: 16
					        font.weight: 600
							font.letterSpacing: 2
							width: 80
					        color: "white"
					    }
					    Text {
					        text: "00:00"
					        font.pixelSize: 14
							width: 80
					        color: "#80ffffff"
					    }
					}
				}
				Item { Layout.fillWidth: true }
				Column { 
					width: 50
				    anchors.verticalCenter: parent.verticalCenter
				    spacing: 2
				    Text {
				        text: "Tura 0"
						horizontalAlignment: Text.AlignHCenter
						width: 50
				        font.pixelSize: 16
				        font.weight: 600
				        color: "white"
				    }
				    Text {
				        text: whosTurn == "PLAYER" ? "Your turn" : "AI's turn"
						horizontalAlignment: Text.AlignHCenter
						width: 50
				        font.pixelSize: 14
				        color: "#80ffffff"
				    }
				}
				Item { Layout.fillWidth: true }
				Row {
					anchors.verticalCenter: parent.verticalCenter
					spacing: 12
					Column { 
					    anchors.verticalCenter: parent.verticalCenter
					    spacing: 2
					    Text {
					        text: "AI (Red)"
					        font.pixelSize: 16
					        font.weight: 600
							horizontalAlignment: Text.AlignRight
							width: 80
							font.letterSpacing: 2
					        color: "white"
					    }
					    Text {
					        text: "00:00"
							horizontalAlignment: Text.AlignRight
							width: 80
					        font.pixelSize: 14
					        color: "#80ffffff"
					    }
					}
					Text {
					    text: "🔴"
					    font.pixelSize: 16
					    anchors.verticalCenter: parent.verticalCenter
					}
				}
			}
		}

		Item { Layout.fillHeight: true }
		//Repeater {
			//model: Board.matrix
			Hexagon {
				width: 20
				height: 20
			}
		//}
		
		Item { Layout.fillHeight: true }
		SecondaryBtn {
			icon: "🚩"
			text: "Resign"
			Layout.fillWidth: true
		}
	}
}