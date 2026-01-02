cmake_minimum_required(VERSION 3.20)

project(Photolab VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick ShaderTools GuiPrivate)

qt_standard_project_setup(REQUIRES 6.5)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_AUTORCC ON)

qt_add_executable(appPhotolab
    main.cpp
    resources.qrc

    controller/controller.h
    controller/controller.cc
    controller/backend_interface.h

    backend/backend.h
    backend/backend.cc
)

add_subdirectory(gui/Constant)
target_link_libraries(appPhotolab PRIVATE ConstantLibplugin)

add_subdirectory(gui/Effects)
target_link_libraries(appPhotolab PRIVATE EffectsLibplugin)

target_include_directories(appPhotolab PRIVATE gui)

qt_add_qml_module(appPhotolab
    URI Photolab
    VERSION 1.0
    QML_FILES
        gui/Main.qml
        gui/ViewSpace.qml
        gui/MenuBar.qml
        gui/PicturePlane.qml
        gui/PictureHint.qml

    SOURCES
        gui/picture_render.h
        gui/picture_render.cc
        gui/pictureplane_item.h
        # RESOURCES gui/Constant/CMakeLists.txt
        # RESOURCES gui/Effects/CMakeLists.txt
)

qt6_add_shaders(appPhotolab "appPhotolab_shaders"
    PREFIX "/shaders"
    BASE "gui/Shaders"
    FILES
        gui/Shaders/Tile.frag
        gui/Shaders/Blur.frag
        gui/Shaders/Radius.frag

        gui/Shaders/PictureRender.vert
        gui/Shaders/PictureRender.frag
)

list(APPEND QML_DIRS ${CMAKE_SOURCE_DIR})
set(QML_IMPORT_PATH "${QML_DIRS}" CACHE STRING "Qt")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPhotolab PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPhotolab
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPhotolab PRIVATE
    Qt6::Quick
    Qt6::GuiPrivate
)

include(GNUInstallDirs)
install(TARGETS appPhotolab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
