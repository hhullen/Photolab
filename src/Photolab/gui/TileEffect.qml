import QtQuick 2.15 as Core
import "Constant" as Const

Core.ShaderEffect {
    anchors.fill: parent
    property real tileSize: Const.Common.tileSize
    property real areaWidth: parent.width
    property real areaHeight: parent.height
    property real borderRradius: 40

    property var src: Core.ShaderEffectSource {
        live: false
        smooth: false
        sourceItem: Core.Item {
            id: tileItem
            width: tileSize
            height: tileSize
            property int halfSize: tileSize / 2

            Core.Rectangle {
                x: 0;
                y: 0;
                width: tileItem.halfSize;
                height: tileItem.halfSize;
                color: Const.Common.tileLight
            }
            Core.Rectangle {
                x: tileItem.halfSize
                y: 0; width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.Common.tileDark
            }
            Core.Rectangle {
                x: 0
                y: tileItem.halfSize
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.Common.tileDark
            }
            Core.Rectangle {
                x: tileItem.halfSize
                y: tileItem.halfSize
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.Common.tileLight
            }
        }
    }

    fragmentShader: "qrc:/shaders/Tile.frag.qsb"

}
