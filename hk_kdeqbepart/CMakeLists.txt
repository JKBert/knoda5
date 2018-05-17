########### next target ###############

set(hk_kdeqbepart_PART_SRCS hk_kdeqbepart.cpp)

add_library(hk_kde5qbepart ${hk_kdeqbepart_PART_SRCS})

target_link_libraries(hk_kde5qbepart
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB} 
    ${LIB_MODULE_NAME})

install(TARGETS hk_kde5qbepart DESTINATION ${KDE_INSTALL_PLUGINDIR})


########### install files ###############

install(FILES  hk_kde5qbepart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdeqbepart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdeqbepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
