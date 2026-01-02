pragma Singleton
import QtQuick 2.15


QtObject {
    readonly property real blurRadius: 1.5
    readonly property real blurBrightness: 0.8

    readonly property string blurShader: "qrc:/shaders/Blur.frag.qsb"
    readonly property string radiusShader:"qrc:/shaders/Radius.frag.qsb"
    readonly property string tileShader: "qrc:/shaders/Tile.frag.qsb"
}
