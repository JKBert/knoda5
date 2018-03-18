
#add_subdirectory(po)
#add_subdirectory(doc)
#add_subdirectory(pics)


########### next target ###############

set(knoda_SRCS
    main.cpp
    hk_kdedriverselectbase.cpp
    hk_kdedriverselect.cpp)

#kde4_add_app_icon(knoda_SRCS "hi*-app-knoda4.png")

add_executable(knoda5 ${knoda_SRCS})

target_link_libraries(knoda5 
    Qt5::Widgets
    KF5::CoreAddons
    KF5::KDELibs4Support
    ${HK_CLASSES_LIB}
    ${LIBRARY_OUTPUT_PATH}/libhk_kde5classes.so)


install(TARGETS knoda5 ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

#set(knoda_rt_SRCS main-rt.cpp)

#kde4_add_app_icon(knoda_rt_SRCS "hi*-app-knoda4.png")

#kde4_add_executable(knoda4_rt ${knoda_rt_SRCS})

#add_dependencies(knoda4_rt hk_kde4classes)

#target_link_libraries(knoda4_rt ${KDE4_KDECORE_LIBS} ${LIBRARY_OUTPUT_PATH}/libhk_kde4classes.so ${HK_CLASSES_LIB} ${KDE4_KPARTS_LIBS})

#install(TARGETS knoda4_rt ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES  hk_classes.magic DESTINATION ${CONFIG_INSTALL_DIR}/magic)
install(FILES knoda4-mime.xml DESTINATION ${XDG_MIME_INSTALL_DIR})
update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})

#install(FILES  knoda4.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

#kde4_install_icons(${ICON_INSTALL_DIR})
