
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(hk_kdedbdesignerpart_PART_SRCS hk_kdedbdesignerpart.cpp)

kde4_add_plugin(hk_kde4dbdesignerpart WITH_PREFIX ${hk_kdedbdesignerpart_PART_SRCS})

target_link_libraries(hk_kde4dbdesignerpart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde4classes)

install(TARGETS hk_kde4dbdesignerpart DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install(FILES  hk_kde4dbdesignerpart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  hk_kdedbdesignerpart.rc DESTINATION ${DATA_INSTALL_DIR}/hk_kde4classes)
install(FILES  hk_kdedbdesignerpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde4classes CONFIGURATIONS Debug)
