
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(hk_kdemodulepart_PART_SRCS hk_kdemodulepart.cpp)

kde4_add_plugin(hk_kde4modulepart WITH_PREFIX ${hk_kdemodulepart_PART_SRCS})

# add_dependencies(hk_kdemodulepart hk_kdeclasses)

target_link_libraries(hk_kde4modulepart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

install(TARGETS hk_kde4modulepart DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install(FILES  hk_kde4modulepart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  hk_kdemodulepart.rc  DESTINATION ${DATA_INSTALL_DIR}/hk_kde4classes)
install(FILES  hk_kdemodulepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde4classes CONFIGURATIONS Debug)
