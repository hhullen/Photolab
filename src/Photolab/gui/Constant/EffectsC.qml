pragma Singleton
import QtQuick 2.15


QtObject {
    readonly property real blurRadius: 1.5
    readonly property rect blurTone: Qt.rect(1.0, 30/255, 220/255, 1.0)

    readonly property string blurShader: "qrc:/shaders/Blur.frag.qsb"
    readonly property string radiusShader:"qrc:/shaders/Radius.frag.qsb"
    readonly property string tileShader: "qrc:/shaders/Tile.frag.qsb"
}
