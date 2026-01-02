qt_add_library(EffectsLib STATIC)

set_target_properties(EffectsLib PROPERTIES AUTOMOC ON)
target_link_libraries(EffectsLib PRIVATE Qt6::Quick)

list(APPEND MODULE_QML_FILES Blur.qml Radius.qml Tile.qml)

set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_qml_module(EffectsLib
    URI Effects
    VERSION 1.0
    QML_FILES
        ${MODULE_QML_FILES}
)
