import QtQuick 2.15 as Core

import Constant as Const

Core.ShaderEffect {
    id: shaderEffect
    anchors.fill: parent
    property real tileSize: Const.ViewSpaceC.tileSize
    property real areaWidth: parent.width
    property real areaHeight: parent.height

    property var src: Core.ShaderEffectSource {
        live: false
        smooth: false
        sourceItem: Core.Item {
            id: tileItem
            width: shaderEffect.tileSize
            height: shaderEffect.tileSize
            property int halfSize: shaderEffect.tileSize / 2

            Core.Rectangle {
                x: 0
                y: 0
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.ViewSpaceC.tileLight
            }
            Core.Rectangle {
                x: tileItem.halfSize
                y: 0
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.ViewSpaceC.tileDark
            }
            Core.Rectangle {
                x: 0
                y: tileItem.halfSize
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.ViewSpaceC.tileDark
            }
            Core.Rectangle {
                x: tileItem.halfSize
                y: tileItem.halfSize
                width: tileItem.halfSize
                height: tileItem.halfSize
                color: Const.ViewSpaceC.tileLight
            }
        }
    }

    fragmentShader: Const.EffectsC.tileShader
}
