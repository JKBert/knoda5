
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(hk_kdeqbepart_PART_SRCS hk_kdeqbepart.cpp)

kde4_add_plugin(hk_kde4qbepart WITH_PREFIX ${hk_kdeqbepart_PART_SRCS})

target_link_libraries(hk_kde4qbepart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

install(TARGETS hk_kde4qbepart DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install(FILES  hk_kde4qbepart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  hk_kdeqbepart.rc DESTINATION ${DATA_INSTALL_DIR}/hk_kde4classes)
install(FILES  hk_kdeqbepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde4classes CONFIGURATIONS Debug)
