import QtQuick 2.15 as Core
import Photolab as App

import Constant as Const

Core.Rectangle {
    id: picturePlane
    color: "transparent"
    width: 1920
    height: 1080
    antialiasing: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    property real cutter: 0.5

    onCutterChanged: {
        picturePlaneItem.cutter = picturePlane.cutter
    }

    App.PicturePlaneItem {
        id: picturePlaneItem
        anchors.fill: picturePlane
    }

    Core.DragHandler {
        acceptedButtons: Qt.LeftButton
        target: picturePlane
    }

    function fitInViewSpace() {
        let scaleX = picturePlane.parent.width / picturePlane.width
        let scaleY = picturePlane.parent.height / picturePlane.height

        let fitScale = Math.min(scaleX, scaleY)

        picturePlane.scale = fitScale * 0.9

        picturePlane.x = (picturePlane.parent.width - picturePlane.width) / 2
        picturePlane.y = (picturePlane.parent.height - picturePlane.height) / 2
    }

    Core.Connections {
        target: picturePlane.parent
        function onWidthChanged() { picturePlane.fitInViewSpace() }
        function onHeightChanged() { picturePlane.fitInViewSpace() }
    }

    function handleZoom(event): void {
        let scaleStep = Const.ViewSpaceC.scaleStep
        let zoomFactor = event.angleDelta.y > 0 ? scaleStep : 1 / scaleStep

        let oldScale = picturePlane.scale
        let nextScale = oldScale * zoomFactor

        let leadingDimension = Math.max(picturePlane.width, picturePlane.height)
        let parentLeadingDimension = (picturePlane.width > picturePlane.height)
                                     ? picturePlane.parent.width
                                     : picturePlane.parent.height

        let minScale = (parentLeadingDimension * Const.ViewSpaceC.scaleMin) / leadingDimension
        let maxScale = Const.ViewSpaceC.scaleMax

        if (nextScale < minScale) nextScale = minScale
        if (nextScale > maxScale) nextScale = maxScale

        if (nextScale === oldScale) return

        let mouseX = event.x
        let mouseY = event.y
        let centerX = picturePlane.width / 2
        let centerY = picturePlane.height / 2

        let dx = (mouseX - centerX) * (oldScale - nextScale)
        let dy = (mouseY - centerY) * (oldScale - nextScale)

        picturePlane.x += dx
        picturePlane.y += dy
        picturePlane.scale = nextScale
    }

    Core.WheelHandler{
        onWheel: (event) => {
            picturePlane.handleZoom(event)
        }
    }
}
