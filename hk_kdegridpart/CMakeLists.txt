########### next target ###############

set(hk_kdegridpart_PART_SRCS hk_kdegridpart.cpp)

add_library(hk_kde5gridpart ${hk_kdegridpart_PART_SRCS})

target_link_libraries(hk_kde5gridpart 
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB} 
    ${LIB_MODULE_NAME})

install(TARGETS hk_kde5gridpart DESTINATION ${QT_PLUGINS_DIR})


########### install files ###############

install(FILES  hk_kde5gridpart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdegridpart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdegridpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
