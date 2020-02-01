########### next target ###############

set(hk_kdequerypart_PART_SRCS hk_kdequerypart.cpp)

add_library(hk_kde5querypart ${hk_kdequerypart_PART_SRCS})

target_link_libraries(hk_kde5querypart
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB} 
    ${LIB_MODULE_NAME})

install(TARGETS hk_kde5querypart DESTINATION ${QT_PLUGINS_DIR})


########### install files ###############

install(FILES  hk_kde5querypart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdequerypart.rc hk_kdequerypartqbe.rc hk_kdequerypartsql.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdequerypart.h DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
