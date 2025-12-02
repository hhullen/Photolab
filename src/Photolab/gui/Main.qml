import QtQuick 2.15 as Core
import "Constant" as Const
import QtQuick.Controls as Controls

Core.Window {
    id: mainWindow
    width: Const.Common.minWidth
    height: Const.Common.minHeight
    minimumWidth: Const.Common.minWidth
    minimumHeight: Const.Common.minHeight
    visible: true
    color: mainWindow.palette.window
    title: qsTr("Hello World")

    Core.Rectangle {
        anchors.fill: parent
        anchors.margins: 5
        color: "transparent"

        TileEffect {}
    }
}
