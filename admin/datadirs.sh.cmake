#!/bin/bash

IFS=':'
read -r USERDATADIR EXT <<< "`kf5-config --path data`"
read -r USERICONDIR EXT <<< "`kf5-config --path icon`"
read -r USERMODULEDIR EXT <<< "`kde4-config --path module`"
read -r USERSERVICESDIR EXT <<< "`kde4-config --path services`"

echo Package root directory: ${CMAKE_SOURCE_DIR}
echo KF5 user data directory: "$USERDATADIR"
echo KF5 user icon directory: "$USERICONDIR"
echo KDE4 user module directory: "$USERMODULEDIR"
echo KDE4 user services directory: "$USERSERVICESDIR"

case "$1" in
    link)
        ln -s ${CMAKE_SOURCE_DIR}/knoda/ $USERDATADIR/knoda5
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeclasses/ $USERDATADIR/hk_kde5classes
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdemodulepart/hk_kdemodulepart.rc $USERDATADIR/hk_kde5classes/hk_kdemodulepart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdetablepart/hk_kdetablepart.rc $USERDATADIR/hk_kde5classes/hk_kdetablepart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdegridpart/hk_kdegridpart.rc $USERDATADIR/hk_kde5classes/hk_kdegridpart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kdeformpartdesign.rc $USERDATADIR/hk_kde5classes/hk_kdeformpartdesign.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kdeformpartview.rc $USERDATADIR/hk_kde5classes/hk_kdeformpartview.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdereportpart/hk_kdereportpartdesign.rc $USERDATADIR/hk_kde5classes/hk_kdereportpartdesign.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdereportpart/hk_kdereportpartview.rc $USERDATADIR/hk_kde5classes/hk_kdereportpartview.rc
# no need to link hk_kdeform.rc as it is located in hk_kdeclasses directory
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdedbdesigner/hk_kdedbdesignerpart.rc $USERDATADIR/hk_kde5classes/hk_kdedbdesignerpart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypartqbe.rc $USERDATADIR/hk_kde5classes/hk_kdequerypartqbe.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypart.rc $USERDATADIR/hk_kde5classes/hk_kdequerypart.rc
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kdequerypartsql.rc $USERDATADIR/hk_kde5classes/hk_kdequerypartsql.rc
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
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdegridpart/hk_kde4gridpart.desktop $USERSERVICESDIR/hk_kde4gridpart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4gridpart.so $USERMODULEDIR/libhk_kde4gridpart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdedbdesigner/hk_kde4dbdesignerpart.desktop $USERSERVICESDIR/hk_kde4dbdesignerpart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4dbdesignerpart.so $USERMODULEDIR/libhk_kde4dbdesignerpart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeformpart/hk_kde4formpart.desktop $USERSERVICESDIR/hk_kde4formpart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4formpart.so $USERMODULEDIR/libhk_kde4formpart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdemodulepart/hk_kde4modulepart.desktop $USERSERVICESDIR/hk_kde4modulepart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4modulepart.so $USERMODULEDIR/libhk_kde4modulepart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdeqbepart/hk_kde4qbepart.desktop $USERSERVICESDIR/hk_kde4qbepart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4qbepart.so $USERMODULEDIR/libhk_kde4qbepart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdequerypart/hk_kde4querypart.desktop $USERSERVICESDIR/hk_kde4querypart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4querypart.so $USERMODULEDIR/libhk_kde4querypart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdereportpart/hk_kde4reportpart.desktop $USERSERVICESDIR/hk_kde4reportpart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4reportpart.so $USERMODULEDIR/libhk_kde4reportpart.so
        ln -s ${CMAKE_SOURCE_DIR}/hk_kdetablepart/hk_kde4tablepart.desktop $USERSERVICESDIR/hk_kde4tablepart.desktop
        ln -s ${LIBRARY_OUTPUT_PATH}/libhk_kde4tablepart.so $USERMODULEDIR/libhk_kde4tablepart.so
# to use user mime types
        ! [ -d ~/.kde4/share/mime ] && mkdir -p ~/.kde4/share/mime/packages
        ln -s ~/.kde4/share/mime ~/.local/share/mime
        ln -s ${CMAKE_SOURCE_DIR}/knoda/knoda4-mime.xml ~/.kde4/share/mime/packages/knoda4-mime.xml
        update-mime-database ~/.local/share/mime/
COMMENT
        exit 0
        ;;
    unlink)
        unlink $USERDATADIR/hk_kde5classes/hk_kdereportpartdesign.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdereportpartview.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdeformpartdesign.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdeformpartview.rc	
        unlink $USERDATADIR/hk_kde5classes/hk_kdetablepart.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdegridpart.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdedbdesignerpart.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdequerypartqbe.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdequerypart.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdequerypartsql.rc
        unlink $USERDATADIR/hk_kde5classes/hk_kdemodulepart.rc
        unlink $USERDATADIR/knoda5
        unlink $USERDATADIR/hk_kde5classes
# icons
        unlink $USERICONDIR/hicolor/32x32/apps/knoda5.png
        unlink $USERICONDIR/hicolor/16x16/apps/knoda5.png
        unlink $USERICONDIR/locolor/32x32/apps/knoda5.png
        unlink $USERICONDIR/locolor/16x16/apps/knoda5.png
: << 'COMMENT' TBP
# services
        unlink $USERSERVICESDIR/hk_kde4gridpart.desktop
        unlink $USERMODULEDIR/libhk_kde4gridpart.so
        unlink $USERSERVICESDIR/hk_kde4dbdesignerpart.desktop
        unlink $USERMODULEDIR/libhk_kde4dbdesignerpart.so
        unlink $USERSERVICESDIR/hk_kde4formpart.desktop
        unlink $USERMODULEDIR/libhk_kde4formpart.so
        unlink $USERSERVICESDIR/hk_kde4modulepart.desktop
        unlink $USERMODULEDIR/libhk_kde4modulepart.so
        unlink $USERSERVICESDIR/hk_kde4qbepart.desktop
        unlink $USERMODULEDIR/libhk_kde4qbepart.so
        unlink $USERSERVICESDIR/hk_kde4querypart.desktop
        unlink $USERMODULEDIR/libhk_kde4querypart.so
        unlink $USERSERVICESDIR/hk_kde4reportpart.desktop
        unlink $USERMODULEDIR/libhk_kde4reportpart.so
        unlink $USERSERVICESDIR/hk_kde4tablepart.desktop
        unlink $USERMODULEDIR/libhk_kde4tablepart.so
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
