#!/bin/bash

IFS=':'
read -r USERDATADIR EXT <<< "`kf5-config --path data`"
read -r USERICONDIR EXT <<< "`kf5-config --path icon`"
read -r USERMODULEDIR EXT <<< "`kf5-config --path module`"
read -r USERSERVICESDIR EXT <<< "`kf5-config --path services`"

echo Package root directory: ${CMAKE_SOURCE_DIR}
echo KF5 user data directory: "$USERDATADIR"
echo KF5 user icon directory: "$USERICONDIR"
echo KF5 user module directory: "$USERMODULEDIR"
echo KF5 user services directory: "$USERSERVICESDIR"

KXMLGUIDIR=$USERDATADIR/kxmlgui5
RCDATADIR=$KXMLGUIDIR/${LIB_MODULE_NAME}

case "$1" in
    link)
        ln -s ${CMAKE_SOURCE_DIR}/knoda/ $USERDATADIR/${PROJECT_NAME}
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeclasses/ $USERDATADIR/${LIB_MODULE_NAME}
        mkdir -p $KXMLGUIDIR
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeclasses/ $RCDATADIR
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdemodulepart/hk_kdemodulepart.rc $RCDATADIR/hk_kdemodulepart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdetablepart/hk_kdetablepart.rc $RCDATADIR/hk_kdetablepart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdegridpart/hk_kdegridpart.rc $RCDATADIR/hk_kdegridpart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kdeformpartdesign.rc $RCDATADIR/hk_kdeformpartdesign.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kdeformpartview.rc $RCDATADIR/hk_kdeformpartview.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdereportpart/hk_kdereportpart.rc $RCDATADIR/hk_kdereportpart.rc
# no need to link hk_kdeform.rc as it is located in hk_kdeclasses directory
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdedbdesigner/hk_kdedbdesignerpart.rc $RCDATADIR/hk_kdedbdesignerpart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypartqbe.rc $RCDATADIR/hk_kdequerypartqbe.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypart.rc $RCDATADIR/hk_kdequerypart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypartsql.rc $RCDATADIR/hk_kdequerypartsql.rc
# icons
        ! [ -d $USERICONDIR/hicolor/32x32/apps ] && mkdir -p $USERICONDIR/hicolor/32x32/apps
        ! [ -d $USERICONDIR/hicolor/16x16/apps ] && mkdir -p $USERICONDIR/hicolor/16x16/apps
        ! [ -d $USERICONDIR/locolor/32x32/apps ] && mkdir -p $USERICONDIR/locolor/32x32/apps
        ! [ -d $USERICONDIR/locolor/16x16/apps ] && mkdir -p $USERICONDIR/locolor/16x16/apps
        ln -s ${CMAKE_SOURCE_DIR}/knoda/pics/icons/hicolor/32-apps-knoda5.png $USERICONDIR/hicolor/32x32/apps/knoda5.png
        ln -s ${CMAKE_SOURCE_DIR}/knoda/pics/icons/hicolor/16-apps-knoda5.png $USERICONDIR/hicolor/16x16/apps/knoda5.png
        ln -s ${CMAKE_SOURCE_DIR}/knoda/pics/icons/locolor/32-apps-knoda5.png $USERICONDIR/locolor/32x32/apps/knoda5.png
        ln -s ${CMAKE_SOURCE_DIR}/knoda/pics/icons/locolor/16-apps-knoda5.png $USERICONDIR/locolor/16x16/apps/knoda5.png
: << 'COMMENT' TBP
# services
        ! [ -d $USERMODULEDIR ] && mkdir -p $USERMODULEDIR
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5dbdesignerpart.so $USERMODULEDIR/libhk_kde5dbdesignerpart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5formpart.so $USERMODULEDIR/libhk_kde5formpart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5qbepart.so $USERMODULEDIR/libhk_kde5qbepart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5querypart.so $USERMODULEDIR/libhk_kde5querypart.so
COMMENT
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5tablepart.so ${CMAKE_BINARY_DIR}/knoda/libhk_kde5tablepart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5gridpart.so ${CMAKE_BINARY_DIR}/knoda/libhk_kde5gridpart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5reportpart.so ${CMAKE_BINARY_DIR}/knoda/libhk_kde5reportpart.so
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde5modulepart.so ${CMAKE_BINARY_DIR}/knoda/libhk_kde5modulepart.so

        ! [ -d $USERSERVICESDIR ] && mkdir -p $USERSERVICESDIR
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdegridpart/hk_kde5gridpart.desktop $USERSERVICESDIR/hk_kde5gridpart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdedbdesigner/hk_kde5dbdesignerpart.desktop $USERSERVICESDIR/hk_kde5dbdesignerpart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kde5formpart.desktop $USERSERVICESDIR/hk_kde5formpart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdemodulepart/hk_kde5modulepart.desktop $USERSERVICESDIR/hk_kde5modulepart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeqbepart/hk_kde5qbepart.desktop $USERSERVICESDIR/hk_kde5qbepart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kde5querypart.desktop $USERSERVICESDIR/hk_kde5querypart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdereportpart/hk_kde5reportpart.desktop $USERSERVICESDIR/hk_kde5reportpart.desktop
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdetablepart/hk_kde5tablepart.desktop $USERSERVICESDIR/hk_kde5tablepart.desktop
: << 'COMMENT' TBP
# to use user mime types
        ! [ -d ~/.kde4/share/mime ] && mkdir -p ~/.kde4/share/mime/packages
        ln -s ~/.kde4/share/mime ~/.local/share/mime
        ln -s ${CMAKE_SOURCE_DIR}/knoda/knoda4-mime.xml ~/.kde4/share/mime/packages/knoda4-mime.xml
        update-mime-database ~/.local/share/mime/
COMMENT
        exit 0
        ;;
    unlink)
        unlink $RCDATADIR/hk_kdereportpart.rc
        unlink $RCDATADIR/hk_kdeformpartdesign.rc
        unlink $RCDATADIR/hk_kdeformpartview.rc	
        unlink $RCDATADIR/hk_kdetablepart.rc
        unlink $RCDATADIR/hk_kdegridpart.rc
        unlink $RCDATADIR/hk_kdedbdesignerpart.rc
        unlink $RCDATADIR/hk_kdequerypartqbe.rc
        unlink $RCDATADIR/hk_kdequerypart.rc
        unlink $RCDATADIR/hk_kdequerypartsql.rc
        unlink $RCDATADIR/hk_kdemodulepart.rc
        unlink $RCDATADIR
        unlink $USERDATADIR/knoda5
        unlink $USERDATADIR/${LIB_MODULE_NAME}
# icons
        unlink $USERICONDIR/hicolor/32x32/apps/knoda5.png
        unlink $USERICONDIR/hicolor/16x16/apps/knoda5.png
        unlink $USERICONDIR/locolor/32x32/apps/knoda5.png
        unlink $USERICONDIR/locolor/16x16/apps/knoda5.png
: << 'COMMENT' TBP
# services       
        unlink $USERMODULEDIR/libhk_kde5dbdesignerpart.so
        unlink $USERMODULEDIR/libhk_kde5formpart.so
        unlink $USERMODULEDIR/libhk_kde5qbepart.so
        unlink $USERMODULEDIR/libhk_kde5querypart.so
        
COMMENT
        unlink ${CMAKE_BINARY_DIR}/knoda/libhk_kde5tablepart.so
        unlink ${CMAKE_BINARY_DIR}/knoda/libhk_kde5gridpart.so
        unlink ${CMAKE_BINARY_DIR}/knoda/libhk_kde5reportpart.so
        unlink ${CMAKE_BINARY_DIR}/knoda/libhk_kde5modulepart.so

        unlink $USERSERVICESDIR/hk_kde5gridpart.desktop
        unlink $USERSERVICESDIR/hk_kde5dbdesignerpart.desktop
        unlink $USERSERVICESDIR/hk_kde5formpart.desktop
        unlink $USERSERVICESDIR/hk_kde5modulepart.desktop
        unlink $USERSERVICESDIR/hk_kde5qbepart.desktop
        unlink $USERSERVICESDIR/hk_kde5querypart.desktop
        unlink $USERSERVICESDIR/hk_kde5reportpart.desktop
        unlink $USERSERVICESDIR/hk_kde5tablepart.desktop
: << 'COMMENT' TBP
# user mime types
        unlink ~/.kde4/share/mime/packages/knoda4-mime.xml
        update-mime-database ~/.local/share/mime/
        unlink ~/.local/share/mime
COMMENT
        exit 0
        ;;
    *)
        echo "Usage: $0 {link|unlink|}"
        exit 1
        ;;
esac
