########### next target ###############

set(hk_kdetablepart_PART_SRCS hk_kdetablepart.cpp)

kde4_add_plugin(hk_kde4tablepart WITH_PREFIX ${hk_kdetablepart_PART_SRCS})

target_link_libraries(hk_kde4tablepart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

install(TARGETS hk_kde4tablepart DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install(FILES  hk_kde4tablepart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  hk_kdetablepart.rc DESTINATION ${DATA_INSTALL_DIR}/hk_kde4classes)
install(FILES  hk_kdetablepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde4classes CONFIGURATIONS Debug)
