pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property int minWidth: 640
    readonly property int minHeight: 480
    readonly property int tileSize: 64
    readonly property color tileLight: Qt.rgba(1.0, 1.0, 1.0, 1.0)
    readonly property color tileDark: Qt.rgba(230/255, 230/255, 230/255, 1.0)

    // readonly property string tileShader : "qrc:/shaders/gui/Shaders/Tile.frag.qsb"
}
