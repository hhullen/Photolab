qt_add_library(ConstantLib STATIC)

set_target_properties(ConstantLib PROPERTIES AUTOMOC ON)
target_link_libraries(ConstantLib PRIVATE Qt6::Quick)

list(APPEND MODULE_QML_FILES Common.qml EffectsC.qml ViewSpaceC.qml)

set_source_files_properties(${MODULE_QML_FILES} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_qml_module(ConstantLib
    URI Constant
    VERSION 1.0
    QML_FILES
        ${MODULE_QML_FILES}
)
