########### next target ###############

set(hk_kdereportpart_PART_SRCS hk_kdereportpart.cpp)

add_library(hk_kde5reportpart ${hk_kdereportpart_PART_SRCS})

target_link_libraries(hk_kde5reportpart 
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB}
    hk_kde5classes)

install(TARGETS hk_kde5reportpart DESTINATION ${QT_PLUGINS_DIR})

########### install files ###############

install(FILES  hk_kde5reportpart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdereportpart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdereportpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
