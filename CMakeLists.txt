cmake_minimum_required(VERSION 3.0)

project(knoda5)
set(PACKAGE_VERSION 0.12.1r1)
set(LIB_MODULE_NAME hk_kde5classes)

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.2.0")

find_package(LibXml2 REQUIRED)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# qmake-qt5 is a must for KDEInstallDirs to set up directories correctly
find_program(QMAKEQT5 qmake-qt5)
if(QMAKEQT5)
  message(STATUS "Found qmake-qt5: " ${QMAKEQT5})
else()
  message(FATAL_ERROR "Not found qmake-qt5")
endif()

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMInstallIcons)
include(ECMAddAppIcon)

# Find Qt modules
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication 
)

# Find KDE modules
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    WidgetsAddons   # KMessageBox
    KDELibs4Support # Porting from KDE4
    DocTools        # Handbook
    ConfigWidgets   # Help invokation
    TextEditor      # Text Editor part
    Parts           # KParts
)

# execute the command "qmake -query QT_INSTALL_PLUGINS" to get the path of plugins dir.
execute_process(COMMAND ${QMAKEQT5} -query QT_INSTALL_PLUGINS OUTPUT_VARIABLE QT_PLUGINS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
if(QT_PLUGINS_DIR) 
  message(STATUS "Qt5 plugin directory:" "${QT_PLUGINS_DIR}")
else() 
  message(FATAL_ERROR "Qt5 plugin directory cannot be detected.")
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# STL needed for hk_string and assignments to/from QString
remove_definitions(-DQT_NO_STL)

add_definitions(-DHAVE_CONFIG_H)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_definitions(-D_DEBUG_ )
endif()

# defines update_xdg_mimetypes macro to update mimetypes after install
include(FindSharedMimeInfo)

include_directories(${LIBXML2_INCLUDE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/hk_kdeclasses
    ${CMAKE_CURRENT_BINARY_DIR})


find_path(HK_CLASSES_INCLUDE "hk_class.h" PATHS "/usr/include/hk_classes" )

if(HK_CLASSES_INCLUDE)
  message(STATUS "Found hk_classes include directory: " ${HK_CLASSES_INCLUDE})
  include_directories(${HK_CLASSES_INCLUDE})
else()
  message(FATAL_ERROR "hk_classes include path not found !")
endif()

find_library(HK_CLASSES_LIB NAMES hk_classes libhk_classes  PATHS "/usr/lib/hk_classes" "/usr/lib64/hk_classes" )

if(HK_CLASSES_LIB)
  message(STATUS "Found hk_classes library: " ${HK_CLASSES_LIB})
else()
  message(FATAL_ERROR "hk_classes library not found !")
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib64)
else()
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
endif()


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(admin)
add_subdirectory(hk_kdeclasses)
add_subdirectory(hk_kdedbdesigner)
add_subdirectory(hk_kdeformpart)
add_subdirectory(hk_kdegridpart)
add_subdirectory(hk_kdemodulepart)
add_subdirectory(hk_kdeqbepart)
add_subdirectory(hk_kdequerypart)
add_subdirectory(hk_kdereportpart)
add_subdirectory(hk_kdetablepart)
add_subdirectory(knoda)

message( STATUS "Configuration is " ${CMAKE_BUILD_TYPE} )
message( STATUS "Install prefix is " ${CMAKE_INSTALL_PREFIX} )
message( STATUS "Architecture is " ${CMAKE_SYSTEM_PROCESSOR})
