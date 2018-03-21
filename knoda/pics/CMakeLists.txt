########### install files ###############

install(FILES  knodalogo.png  DESTINATION ${DATA_INSTALL_DIR}/knoda5/pics)

file(GLOB ICONS_HI_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hicolor/*-apps-knoda5.png")
ecm_install_icons( ICONS ${ICONS_HI_SRCS} DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor )

file(GLOB ICONS_LO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/locolor/*-apps-knoda5.png")
ecm_install_icons( ICONS ${ICONS_LO_SRCS} DESTINATION ${KDE_INSTALL_ICONDIR} THEME locolor )
