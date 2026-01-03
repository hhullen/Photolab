import QtQuick 2.15 as Core

import Constant as Const
import Effects as Effects

Core.Rectangle {
    id: sliderHandle
    width: 30
    height: 20
    color: "transparent"

    property var containerBackground: null
    property var object: null

    readonly property real visualWidth: object.width * object.scale
    readonly property real visualHeight: object.height * object.scale

    readonly property real visualLeft: object.x + (object.width - visualWidth) / 2
    readonly property real visualBottom: object.y + (object.height - visualHeight) / 2 + visualHeight

    x: visualLeft + (visualWidth * object.cutter) - (width / 2)

    y: visualBottom + 3

    Core.MouseArea {
        anchors.fill: parent
        cursorShape: Qt.SizeHorCursor

        property real startX: 0

        onPressed: {
            startX = mouseX
        }

        onPositionChanged: {
            var delta = mouseX - startX

            var newCutter = object.cutter + (delta / sliderHandle.visualWidth)

            if (newCutter < 0.0) newCutter = 0.0
            if (newCutter > 1.0) newCutter = 1.0

            object.cutter = newCutter
        }
    }

    Effects.Blur{
        backgroundSource: containerBackground
    }
}
