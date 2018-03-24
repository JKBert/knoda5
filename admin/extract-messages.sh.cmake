#!/bin/sh

BASEDIR=`pwd`/`dirname $0`/../..	# root of translatable sources
PROJECT="@CMAKE_PROJECT_NAME@"	# project name
PROJECTVERSION="@PACKAGE_VERSION@" # project version
BUGADDR="http://sourceforge.net/p/knoda5/tickets/"	# MSGID-Bugs
WDIR=${BASEDIR}/knoda/po/		# working dir
HKPROJECT="hk_classes-0.8.4" # root of hk_classes sources
HKCLASSESDIR=`pwd`/`dirname $0`/../../../${HKPROJECT}

# check for extractrc installation
extractrc --help >& /dev/null || { echo "error while calling extractrc. aborting."; exit 1;}

echo "Extracting rc files"
cd ${BASEDIR}
# we use simple sorting to make sure the lines do not jump around too much from system to system
find . -name '*.rc' -o -name '*.ui' -o -name '*.kcfg' | sort > ${WDIR}/rcfiles.list
# Strings for KAboutData
echo 'i18nc("NAME OF TRANSLATORS","Your names");' > ${WDIR}/rc.cpp
echo 'i18nc("EMAIL OF TRANSLATORS","Your emails");' >> ${WDIR}/rc.cpp
xargs --arg-file=${WDIR}/rcfiles.list extractrc >> ${WDIR}/rc.cpp
cd ${WDIR}
xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
	-kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
	-khk_translate -ktr --no-location -o ${PROJECT}.pot rc.cpp
mv rc.cpp rc.cpp.txt
echo "Done extracting rc files"


echo "Extracting hk_classes messages"
cd ${HKCLASSESDIR} || { echo "cannot find hk_classes source tree, aborting."; exit 1;}
find . -name '*.cpp' -o -name '*.h' -o -name '*.c' | sort > ${WDIR}/infileshk.list
cd ${WDIR}
xgettext --from-code=UTF-8 -j -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
	-kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
	-khk_translate --files-from=infileshk.list \
	-D ${HKCLASSESDIR} -D ${WDIR} -o ${PROJECT}.pot || { echo "error while calling xgettext. aborting."; exit 1; }
echo "Done extracting hk_classes messages"

echo "Extracting knoda5 messages"
cd ${BASEDIR}
# see above on sorting
find . -name '*.cpp' -o -name '*.h' -o -name '*.c' | sort > ${WDIR}/infiles.list
cd ${WDIR}
xgettext --from-code=UTF-8 -j -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
	-kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
	-khk_translate -ktr --files-from=infiles.list --package-name="${PROJECT}" \
	--package-version="${PROJECTVERSION}" --msgid-bugs-address="${BUGADDR}" \
	-D ${BASEDIR} -D ${WDIR} -o ${PROJECT}.pot || { echo "error while calling xgettext. aborting."; exit 1; }
echo "Done extracting knoda5 messages"

 
echo "Merging translations"
catalogs=`find . -name '*.po'`
for cat in $catalogs; do
  echo $cat
  msgmerge -o $cat.new $cat ${PROJECT}.pot
  mv $cat.new $cat
done
echo "Done merging translations"
 
 
echo "Cleaning up"
cd ${WDIR}
rm rcfiles.list
rm infiles.list
rm infileshk.list
rm rc.cpp.txt
echo "Done"
