cmake_minimum_required(VERSION 2.8.7)

project(knoda4)
set(PACKAGE_VERSION 0.11.1)

find_package(KDE4 REQUIRED)
find_package(LibXml2 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

# STL needed for hk_string and assignments to/from QString
remove_definitions(-DQT_NO_STL)

add_definitions(-DHAVE_CONFIG_H)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_definitions(-D_DEBUG_ )
endif()

include(KDE4Defaults)

# defines update_xdg_mimetypes macro to update mimetypes after install
include(FindSharedMimeInfo)

include(MacroLibrary)

include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${LIBXML2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hk_kdeclasses)

if(KDE4_KTEXTEDITOR_LIBS)
  message(STATUS "Found ktexteditor libraries: " ${KDE4_KTEXTEDITOR_LIBS})
else()
  message(FATAL_ERROR "ktexteditor libraries not found !")
endif()

find_path(HK_CLASSES_INCLUDE "hk_class.h" PATHS "/usr/include/hk_classes" )

if(HK_CLASSES_INCLUDE)
  message(STATUS "Found hk_classes include directory: " ${HK_CLASSES_INCLUDE})
  include_directories(${HK_CLASSES_INCLUDE})
else()
  message(FATAL_ERROR "hk_classes include path not found !")
endif()

find_library(HK_CLASSES_LIB NAMES hk_classes libhk_classes  PATHS "/usr/lib/hk_classes" )

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

