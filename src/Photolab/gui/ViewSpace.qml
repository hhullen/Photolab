import QtQuick 2.15 as Core
import Constant as Const
import Effects as Effects

Core.Rectangle {
    id: viewSpace
    anchors.fill: parent
    property var topElement: null

    anchors.topMargin: topElement.height
    anchors.margins: 5
    color: "transparent"
    clip: true

    Core.Rectangle {
        id: background
        anchors.fill: viewSpace
        Effects.Tile {}
    }

    property var background: background
}
