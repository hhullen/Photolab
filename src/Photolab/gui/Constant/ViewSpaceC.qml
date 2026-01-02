pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property int tileSize: 64
    readonly property color tileLight: Qt.rgba(1.0, 1.0, 1.0, 1.0)
    readonly property color tileDark: Qt.rgba(200/255, 200/255, 200/255, 1.0)
    readonly property int borderRradius: 40

    readonly property color pictureHintColor: Qt.rgba(255, 255, 255, 1.0)
    readonly property int pictureHintSize: 32
    readonly property int pictureHintRadius: 17

    readonly property real scaleStep: 1.1
    readonly property real scaleMax: 20
    readonly property real scaleMin: 0.2
}
