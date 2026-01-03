import QtQuick 2.15 as Core

import Constant as Const
import Effects as Effects

Core.Window {
    id: mainWindow
    width: Const.Common.minWidth
    height: Const.Common.minHeight
    minimumWidth: Const.Common.minWidth
    minimumHeight: Const.Common.minHeight
    visible: true
    color: mainWindow.palette.window
    title: qsTr(Const.Common.appName)

    MenuBar {
        id: menuBar
    }

    ViewSpace {
        id: viewSpace
        topElement: menuBar

        PicturePlane {
            id: picturePlane
        }

        PictureHint {
            id: pictureHintBackground
            containerBackground: viewSpace.background
            object: picturePlane
        }

        Effects.Radius {
            backgroundSource: pictureHintBackground
            radius: Const.ViewSpaceC.pictureHintRadius
        }

        CutterSlider {
            id: cutterSlider
            containerBackground: viewSpace.background
            object: picturePlane
        }

        Effects.Radius {
            backgroundSource: cutterSlider
            radius: Const.ViewSpaceC.cutterSliderRadius
        }
    }

    Effects.Radius {
        backgroundSource: viewSpace
        radius: Const.ViewSpaceC.borderRradius
    }
}
