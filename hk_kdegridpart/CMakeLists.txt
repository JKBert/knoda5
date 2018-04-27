########### next target ###############

set(hk_kdegridpart_PART_SRCS hk_kdegridpart.cpp)

add_library(hk_kde5gridpart ${hk_kdegridpart_PART_SRCS})

target_link_libraries(hk_kde5gridpart 
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB} 
    hk_kde5classes)

install(TARGETS hk_kde5gridpart DESTINATION ${KDE_INSTALL_PLUGINDIR})


########### install files ###############

install(FILES  hk_kde5gridpart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdegridpart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/hk_kde5classes)
install(FILES  hk_kdegridpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde5classes CONFIGURATIONS Debug)
