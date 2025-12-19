pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property int minWidth: 640
    readonly property int minHeight: 480
    readonly property string appName: "Photolab"

    readonly property variant fileNameFilters: [
        qsTr("Images (*.png *.jpg *.jpeg *.bmp)"),
        qsTr("All (*)")
    ]

}
