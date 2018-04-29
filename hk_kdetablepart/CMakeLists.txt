########### next target ###############

set(hk_kdetablepart_PART_SRCS hk_kdetablepart.cpp)

add_library(hk_kde5tablepart ${hk_kdetablepart_PART_SRCS})

target_link_libraries(hk_kde5tablepart
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB}
    hk_kde5classes)

install(TARGETS hk_kde5tablepart DESTINATION ${KDE_INSTALL_PLUGINDIR})


########### install files ###############

install(FILES  hk_kde5tablepart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdetablepart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/hk_kde5classes)
install(FILES  hk_kdetablepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde5classes CONFIGURATIONS Debug)
