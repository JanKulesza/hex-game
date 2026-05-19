import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Shapes

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "HexGame"
    color: "#060e17"
    StackView {
        id: stack
        anchors.fill: parent
        initialItem: MenuScreen {}
    }
}
