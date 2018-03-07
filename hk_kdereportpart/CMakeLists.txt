
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(hk_kdereportpart_PART_SRCS hk_kdereportpart.cpp)

kde4_add_plugin(hk_kde4reportpart WITH_PREFIX ${hk_kdereportpart_PART_SRCS})

target_link_libraries(hk_kde4reportpart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

install(TARGETS hk_kde4reportpart DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install(FILES  hk_kde4reportpart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  hk_kdereportpartview.rc hk_kdereportpartdesign.rc DESTINATION ${DATA_INSTALL_DIR}/hk_kde4classes)
install(FILES  hk_kdereportpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde4classes CONFIGURATIONS Debug)
