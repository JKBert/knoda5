
#TBP include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### next target ###############

set(hk_kdemodulepart_PART_SRCS hk_kdemodulepart.cpp)

# TBP kde4_add_plugin(hk_kde4modulepart WITH_PREFIX ${hk_kdemodulepart_PART_SRCS})

# add_dependencies(hk_kdemodulepart hk_kdeclasses)

add_library(hk_kde5modulepart ${hk_kdemodulepart_PART_SRCS})

#TBP target_link_libraries(hk_kde5modulepart ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} ${HK_CLASSES_LIB} hk_kde5classes)
target_link_libraries(hk_kde5modulepart 
    KF5::CoreAddons
    KF5::Parts
  # TBP  KF5::KDELibs4Support
 ${HK_CLASSES_LIB} hk_kde5classes)

#TBP install(TARGETS hk_kde5modulepart DESTINATION ${PLUGIN_INSTALL_DIR})
install(TARGETS hk_kde5modulepart DESTINATION ${KDE_INSTALL_PLUGINDIR})


########### install files ###############

#TBP install(FILES  hk_kde5modulepart.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES hk_kde5modulepart.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )

#TBPinstall(FILES  hk_kdemodulepart.rc  DESTINATION ${DATA_INSTALL_DIR}/hk_kde5classes)
install( FILES hk_kdemodulepart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/hk_kde5classes )

install(FILES  hk_kdemodulepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde5classes CONFIGURATIONS Debug)
