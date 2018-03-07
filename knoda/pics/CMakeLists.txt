
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES  knodalogo.png  DESTINATION ${DATA_INSTALL_DIR}/knoda4/pics)



#original Makefile.am contents follow:

## Add all of your pixmaps here
#picsdata_DATA = knodalogo.png 
#
## This is where it will all be installed
#picsdatadir = $(kde_datadir)/knoda/pics
