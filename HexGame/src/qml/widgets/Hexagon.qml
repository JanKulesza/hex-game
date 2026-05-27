import QtQuick
import QtQuick.Shapes

// https://stackoverflow.com/questions/63487369/is-there-a-qt-qml-function-to-check-if-a-shape-has-been-clicked

Item
{
    property real srh // Scale Factor
    	//property string color: "black"
    Shape {
        id: rootShape
        width:  srh * 5.2
        height: srh * 6
        anchors.centerIn: parent
        anchors.fill: parent
        containsMode: Shape.FillContains
    
        ShapePath {
            id: myHexagon
            strokeWidth: 2
            strokeColor: "white"
            fillColor: hoverHandler.hovered ? "gold" : "steelblue"
            
            //Path
            startX:       2.6     * srh;
            startY:                        0            * srh
            PathLine { x: 5.2   * srh;  y: 1.5         * srh }
            PathLine { x: 5.2   * srh;  y: (6.0-1.5)   * srh }
            PathLine { x: 2.6   * srh;  y: 6.0          * srh }
            PathLine { x: 0     * srh;  y: (6.0-1.5)   * srh }
            PathLine { x: 0     * srh;  y: 1.5         * srh }
            PathLine { x: 2.6     * srh;  y: 0            * srh }
        }
        HoverHandler {
            id: hoverHandler
        }
        TapHandler {
            onTapped: print("Hexagon clicked")
        }
    }
}
