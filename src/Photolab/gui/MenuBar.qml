import QtQuick 2.15 as Core
import QtQuick.Controls as Ctrl
import QtQuick.Dialogs as Dialog
import Qt.labs.platform as Platform

import Constant as Const

Ctrl.MenuBar {
    background: Core.Rectangle {
        color: mainWindow.palette.window
    }

    Ctrl.Menu {
        title: qsTr("File")

        Ctrl.MenuItem {
            text: qsTr("Open")
            icon.source: "qrc:/icons/resources/load.png"
            onTriggered: fileDialog.open()
        }

        Ctrl.MenuItem {
            text: qsTr("Save")
            icon.source: "qrc:/icons/resources/save.png"
            enabled: false
        }

        Ctrl.MenuSeparator{}
    }

    Dialog.FileDialog {
        id: fileDialog
        title: qsTr("Open-file-dialog-name")

        nameFilters: Const.Common.fileNameFilters

        currentFolder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.PicturesLocation)


        onAccepted: {
            console.log("Выбранный файл: " + fileDialog.selectedFile)

        }

        onRejected: {
            console.log("Выбор файла отменен")
        }
    }
}
