
########### next target ###############

set(hk_kdemodulepart_PART_SRCS hk_kdemodulepart.cpp)

# add_dependencies(hk_kdemodulepart hk_kdeclasses)

add_library(hk_kde5modulepart ${hk_kdemodulepart_PART_SRCS})

target_link_libraries(hk_kde5modulepart 
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB}
    hk_kde5classes)

install(TARGETS hk_kde5modulepart DESTINATION ${KDE_INSTALL_PLUGINDIR})


########### install files ###############

install( FILES hk_kde5modulepart.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )

install( FILES hk_kdemodulepart.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/hk_kde5classes )

install(FILES  hk_kdemodulepart.h DESTINATION ${INCLUDE_INSTALL_DIR}/hk_kde5classes CONFIGURATIONS Debug)
