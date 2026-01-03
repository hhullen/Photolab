import QtQuick 2.15 as Core

import Constant as Const
import Effects as Effects

Core.Rectangle {
    id: pictureHintBackground
    color: "transparent"

    property var containerBackground: null
    property var object: null

    x: (object.x + object.width / 2) - (object.width / 2 * object.scale)
    y: (object.y + object.height / 2) - (object.height / 2 * object.scale) - height - 3
    width: pictureHint.width + 10
    height: pictureHint.height

    Effects.Blur{
        backgroundSource: containerBackground
    }

    Core.Text {
        id: pictureHint
        anchors.centerIn: parent
        text: Math.round(object.width) + "x" + Math.round(object.height)
        color: Const.ViewSpaceC.pictureHintColor
        font.pixelSize: Const.ViewSpaceC.pictureHintSize
    }
}
