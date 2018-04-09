
########### next target ###############

set(hk_kdereportpart_PART_SRCS hk_kdereportpart.cpp)

add_library(hk_kde5reportpart ${hk_kdereportpart_PART_SRCS})

# TBP kde4_add_plugin(hk_kde4reportpart WITH_PREFIX ${hk_kdereportpart_PART_SRCS})

# TBP target_link_libraries(hk_kde4reportpart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

target_link_libraries(hk_kde5reportpart 
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB}
    hk_kde5classes)

install(TARGETS hk_kde5reportpart DESTINATION ${KDE_INSTALL_PLUGINDIR})

########### install files ###############

install(FILES  hk_kde5reportpart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdereportpart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/hk_kde5classes)
install(FILES  hk_kdereportpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde5classes CONFIGURATIONS Debug)
