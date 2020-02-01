########### next target ###############

set(hk_kdeformpart_PART_SRCS hk_kdeformpart.cpp)

add_library(hk_kde5formpart ${hk_kdeformpart_PART_SRCS})

target_link_libraries(hk_kde5formpart
    KF5::CoreAddons
    KF5::Parts
    ${HK_CLASSES_LIB} 
    ${LIB_MODULE_NAME})

install(TARGETS hk_kde5formpart DESTINATION ${QT_PLUGINS_DIR})


########### install files ###############

install(FILES  hk_kde5formpart.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES  hk_kdeformpartdesign.rc hk_kdeformpartview.rc  DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdeformpart.h DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
