
add_subdirectory(po)
add_subdirectory(doc)
add_subdirectory(pics)


########### next target ###############

set(knoda_SRCS
    main.cpp
    hk_kdedriverselectbase.cpp
    hk_kdedriverselect.cpp)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/pics/icons/hicolor/*-apps-knoda5.png")
ecm_add_app_icon(knoda_SRCS ICONS ${ICONS_SRCS})

add_executable(knoda5 ${knoda_SRCS})

target_link_libraries(knoda5 
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigWidgets
    KF5::I18n
    KF5::KDELibs4Support
    ${HK_CLASSES_LIB}
    ${LIBRARY_OUTPUT_PATH}/lib${LIB_MODULE_NAME}.so)


install(TARGETS knoda5 ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############

set(knoda_rt_SRCS main-rt.cpp)

#TBP kde4_add_app_icon(knoda_rt_SRCS "hi*-app-knoda4.png")
ecm_add_app_icon(knoda_rt_SRCS ICONS ${ICONS_SRCS})

#TBP kde4_add_executable(knoda4_rt ${knoda_rt_SRCS})

#TBP target_link_libraries(knoda4_rt ${KDE4_KDECORE_LIBS} ${LIBRARY_OUTPUT_PATH}/libhk_kde4classes.so ${HK_CLASSES_LIB} ${KDE4_KPARTS_LIBS})

#TBP install(TARGETS knoda4_rt ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES  hk_classes.magic DESTINATION ${CONFIG_INSTALL_DIR}/magic)
install(FILES knoda5-mime.xml DESTINATION ${XDG_MIME_INSTALL_DIR})

#TBP do not update during development to speed up install/unistall processes
#update_xdg_mimetypes(${XDG_MIME_INSTALL_DIR})

install(FILES knoda5.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
