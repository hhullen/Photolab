QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

QMAKE_CXXFLAGS += -std=c++20

release {
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CFLAGS_RELEASE   -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
    QMAKE_CFLAGS_RELEASE   += -O3
}

# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/controller.cc \
    gui/colorcorrectiondialog.cc \
    gui/simplefilterdialog.cc \
    gui/kernelfilterdialog.cc \
    gui/mainwindow.cc \
    gui/pictireplane.cc \
    gui/viewspace.cc \
    image_processor/colorcorrection.cc \
    image_processor/imageprocessor.cc \
    image_processor/simplefilter.cc \
    image_processor/kernelfilter.cc \
    main.cc

HEADERS += \
    controller/controller.h \
    gui/colorcorrectiondialog.h \
    gui/const.h \
    gui/controller_interface.h \
    gui/mainwindow.h \
    gui/pictureplane.h \
    gui/kernelfilterdialog.h \
    gui/simplefilterdialog.h \
    gui/viewspace.h \
    image_processor/channel.h \
    image_processor/colorcorrection.h \
    image_processor/imageprocessor.h \
    image_processor/kernelfilter.h \
    image_processor/runner.h \
    image_processor/simplefilter.h \
    image_processor/support.h \
    libs/matrix/matrix.h

TRANSLATIONS += \
    Photolab_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32: RC_ICONS = resources/win_icon.ico
macx:  ICON = resources/mac_icon.icns
