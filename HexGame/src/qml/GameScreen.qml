import QtQuick
import QtQuick.Layouts
import "widgets"
import App

Item {
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
					        text: Board.playersColor != Game.Color.Blue ? "AI (Red)" : "You (Red)"
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
				        text: "Round: " + Board.round
						horizontalAlignment: Text.AlignHCenter
						width: 50
				        font.pixelSize: 16
				        font.weight: 600
				        color: "white"
				    }
				    Text {
				        text: Board.currentPlayer == Game.Color.Blue 
						? Board.playersColor == Game.Color.Blue ? "Your turn" : "AI's turn"
						: Board.playersColor == Game.Color.Blue ? "AI's turn" : "Your turn"
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
					        text: Board.playersColor == Game.Color.Blue ? "AI (Red)" : "You (Red)"
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

		Item {
			Layout.fillHeight: true
			Layout.fillWidth: true
			Column {
				id: boardRoot
				property real hexSize: 7 * 9.0 / Board.size 
				anchors.centerIn: parent
				// Center in item, recompensate for row translate
				transform: Translate { x: -(boardRoot.hexSize * 5.2 / 2) * (Board.size - 1) / 2.0 }
				Repeater {
				model: Board.size
				delegate: Row { 
							readonly property int rowIndex: index
							transform: Translate { 
							    x: (boardRoot.hexSize * 5.2 / 2) * rowIndex;
								y: -(boardRoot.hexSize * 6 * 0.25) * rowIndex
							}
							Repeater {
								id: rowRep
								model: Board.size
								Hexagon {
									srh: boardRoot.hexSize
									width: boardRoot.hexSize * 5.2
									height: boardRoot.hexSize * 6
									hexId: rowIndex * Board.size + index
								}
							} 
						}
				}
			}
		}
		Text {
			text: Board.notification
			font.pixelSize: 32
			color: "#80ffffff"
			horizontalAlignment: Text.AlignHCenter
			Layout.fillWidth: true
			Layout.margins: 16
		}
		SecondaryBtn {
			icon: "🚩"
			text: "Resign"
			Layout.fillWidth: true
			onClicked: {
				stack.pop()
				stack.pop()
			}
		}
	}
}