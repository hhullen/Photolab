import QtQuick 2.15 as Core

import Constant as Const

Core.ShaderEffect {
    id: rootEffect
    anchors.fill: parent

    property var backgroundSource: null
    property real blurRadius: Const.EffectsC.blurRadius
    property bool isHorizontal: false
    property rect toneColor: Const.EffectsC.blurTone

    property variant src: Core.ShaderEffectSource {
        sourceItem: horizontalPass
        hideSource: true
        live: true
        smooth: true
        recursive: false
    }

    fragmentShader: Const.EffectsC.blurShader

    Core.ShaderEffect {
        anchors.fill: parent
        id: horizontalPass
        width: rootEffect.width
        height: rootEffect.height
        visible: false

        property var backgroundSource: rootEffect.backgroundSource
        property real blurRadius: rootEffect.blurRadius
        property bool isHorizontal: true
        property rect toneColor: Qt.rect(1.0, 1.0, 1.0, 1.0)

        property variant src: Core.ShaderEffectSource {
            sourceItem: horizontalPass.backgroundSource
            hideSource: false
            live: true
            recursive: false
            sourceRect: {
                var forceUpdateX = rootEffect.parent.x
                var forceUpdateY = rootEffect.parent.y

                var p = rootEffect.backgroundSource.mapFromItem(rootEffect.parent, 0, 0)

                return Qt.rect(p.x, p.y, rootEffect.parent.width, rootEffect.parent.height)
            }
        }

        fragmentShader: Const.EffectsC.blurShader
    }
}
