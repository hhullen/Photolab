import QtQuick 2.15 as Core

import Constant as Const

Core.ShaderEffect {
    id: root

    property var backgroundSource: null
    property real radius: Const.ViewSpaceC.borderRradius

    anchors.fill: backgroundSource
    width: backgroundSource.width
    height: backgroundSource.height

    property variant src: Core.ShaderEffectSource {
        sourceItem: root.backgroundSource
        hideSource: true
        live: true
    }

    fragmentShader: Const.EffectsC.radiusShader
}
