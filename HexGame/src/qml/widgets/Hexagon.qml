import QtQuick
import QtQuick.Shapes
import App

// https://stackoverflow.com/questions/63487369/is-there-a-qt-qml-function-to-check-if-a-shape-has-been-clicked

Item
{
    id: root
    property real srh // Scale Factor
    property int hexId
    Shape {
        id: rootShape
        width:  root.srh * 5.2
        height: root.srh * 6
        anchors.centerIn: parent
        containsMode: Shape.FillContains
        opacity: hoverHandler.hovered ? 0.5 : 1.0
        
    
        ShapePath {
            id: myHexagon
            strokeWidth: 2
            strokeColor: "#19222b"
            fillColor: Board.hexagons[hexId].color
            
            //Path
            startX:       2.6     * root.srh;
            startY:                        0            * root.srh
            PathLine { x: 5.2   * root.srh;  y: 1.5         * root.srh }
            PathLine { x: 5.2   * root.srh;  y: (6.0-1.5)   * root.srh }
            PathLine { x: 2.6   * root.srh;  y: 6.0          * root.srh }
            PathLine { x: 0     * root.srh;  y: (6.0-1.5)   * root.srh }
            PathLine { x: 0     * root.srh;  y: 1.5         * root.srh }
            PathLine { x: 2.6     * root.srh;  y: 0            * root.srh }
        }
        HoverHandler {
            id: hoverHandler
            cursorShape: Qt.PointingHandCursor
        }
        TapHandler {
            onTapped: Board.pick(hexId)
        }
    }
}
