
ADD_DEFINITIONS(-DMAKE_KDECLASSES_LIB)

add_subdirectory(pics)

########### next target ###############
set(hk_kdeclasses_LIB_SRCS
    hk_kdeaddtabledialog.cpp
    hk_kdeaddtabledialogbase.cpp
    hk_kdeboolean.cpp
    hk_kdebutton.cpp 
    hk_kdecombobox.cpp
    hk_kdecomboboxtextlist.cpp
    hk_kdecomboboxtextlistbase.cpp
    hk_kdecolumn.cpp
    hk_kdecsvexportdialog.cpp
    hk_kdecsvexportdialogbase.cpp
    hk_kdecsvimportdialog.cpp
    hk_kdecsvimportdialogbase.cpp
    hk_kdedatabasesetting.cpp
    hk_kdedatabasesettingbase.cpp 
    hk_kdedatasource.cpp 
    hk_kdedate.cpp
    hk_kdedbdesigner.cpp 
    hk_kdedblistview.cpp
#TBP    hk_kdedriverdialog.cpp
    hk_kdeeximportdatabasebase.cpp
    hk_kdeeximportdatabase.cpp
    hk_kdefilterdialogbase.cpp
    hk_kdefilterdialog.cpp
    hk_kdefinddialog.cpp
    hk_kdefinddialogbase.cpp
    hk_kdeform.cpp
    hk_kdeformdatasourcedialog.cpp
    hk_kdeformdatasourcedialogbase.cpp
    hk_kdeformfocus.cpp
    hk_kdeformpartwidget.cpp
    hk_kdegrid.cpp
    hk_kdegridcolumndialogbase.cpp
    hk_kdegridcolumndialog.cpp
    hk_kdeimage.cpp
    hk_kdeindexeditwindow.cpp
    hk_kdeinterpreterdialog.cpp
    hk_kdelabel.cpp
    hk_kdelineedit.cpp
    hk_kdememo.cpp
    hk_kdemodule.cpp
    hk_kdemodulepartwidget.cpp 
    hk_kdemessages.cpp
    hk_kdenewdatabase.cpp
    hk_kdenewdatabasebase.cpp
    hk_kdenewpassworddialog.cpp
    hk_kdenewpassworddialogbase.cpp
    hk_kdeobjecthandler.cpp
    hk_kdeobjecthandlerbase.cpp 
    hk_kdepassworddialog.cpp 
    hk_kdepreferencesdialog.cpp
    hk_kdepreferencesdialogbase.cpp
    hk_kdeproperty.cpp
    hk_kdepropertybase.cpp
    hk_kdeqbe.cpp
    hk_kdequery.cpp
    hk_kdequerypartwidget.cpp 
    hk_kderelationdialogbase.cpp
    hk_kderelationdialog.cpp 
    hk_kdereport.cpp
    hk_kdereportconditiondialog.cpp
    hk_kdereportconditiondialogbase.cpp
    hk_kdereportdata.cpp 
    hk_kdereportpartwidget.cpp
    hk_kdereportproperty.cpp
    hk_kdereportpropertybase.cpp
    hk_kdereportsection.cpp
    hk_kdereportsectiondialog.cpp
    hk_kdereportsectiondialogbase.cpp
    hk_kderowselector.cpp
    hk_kdesimpleform.cpp
    hk_kde4simplegrid.cpp
    hk_kde4simplegrid_p.cpp 
    hk_kdesimplereport.cpp
    hk_kdespinbox.cpp
    hk_kdesubform.cpp
    hk_kdesubformdialog.cpp
    hk_kdesubreportdialog.cpp
    hk_kdesubreportdialogbase.cpp
    hk_kdetable.cpp
    hk_kdetaborder.cpp
    hk_kdetaborderbase.cpp
    hk_kdetabledesign.cpp 
    hk_kdetablepartwidget.cpp
    hk_kdetoolbar.cpp
    hk_kdexmlexportdialog.cpp
    hk_kdexmlexportdialogbase.cpp
    knodamaindockwindow.cpp
    knodamaindockwindowbase.cpp 
)

add_library(${LIB_MODULE_NAME} SHARED ${hk_kdeclasses_LIB_SRCS})

target_link_libraries(${LIB_MODULE_NAME} 
    xml2
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    KF5::TextEditor
    KF5::KDELibs4Support
    ${HK_CLASSES_LIB})

set_target_properties(${LIB_MODULE_NAME} PROPERTIES VERSION 1.0.0 SOVERSION 1)
install(TARGETS ${LIB_MODULE_NAME} ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############
install(FILES  hk_kdereport.rc hk_kdeform.rc hk_kdetable.rc hk_kdequery.rc hk_kdeinterpreterdialog.rc hk_kdedbdesigner.rc
               knodamaindockwindowbase.rc hk_kdemodule.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/${LIB_MODULE_NAME})
install(FILES  hk_kdedbdesigner.h hk_kdeformpartwidget.h hk_kdegrid.h hk_kdemessages.h 
               hk_kdemodulepartwidget.h hk_kdepreferencesdialog.h hk_kdeqbe.h hk_kdequerypartwidget.h
               hk_kdereportpartwidget.h hk_kdesimpleform.h hk_kdetablepartwidget.h 
               hk_kde4simplegrid.h knodamaindockwindow.h
               DESTINATION ${INCLUDE_INSTALL_DIR}/${LIB_MODULE_NAME} CONFIGURATIONS Debug)
