import QtQuick 2.15 as Core
import "Constant" as Const
import "Effects" as Effects

Core.Rectangle {
    id: viewSpace
    anchors.fill: parent
    anchors.topMargin: menuBar.height
    anchors.margins: 5
    color: "transparent"
    clip: true


    Effects.Tile {}
}
