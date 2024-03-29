project(plurk)

set(RESOURCE_VERSION "0.1")
set(LIB_VERSION "0.1")
set(LIB_SOVERSION "1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

# Turn this off by default when KDEPIM 4.4 is not in use anymore
option(KDEPIM_44_COMPAT "Don't crash when using KDEPIM runtime 4.4.x, at the cost of using deprecated KCal API" TRUE)
if(KDEPIM_44_COMPAT)
  add_definitions(-DWANT_DEPRECATED_KCAL_API)
endif(KDEPIM_44_COMPAT)

find_package(KdepimLibs 4.5.63 REQUIRED)
find_program(XSLTPROC_EXECUTABLE xsltproc)
macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" TRUE)
set(QJSON_VERSION_REQUIRED 0.7)
find_package(QJSON ${QJSON_VERSION_REQUIRED})
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE "${QJSON_VERSION_REQUIRED}")
find_package(QtOAuth REQUIRED)
find_package(QCA2 REQUIRED)

configure_file(config.h.cmake "${CMAKE_CURRENT_BINARY_DIR}/config.h")

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions (-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDEPIMLIBS_INCLUDE_DIRS}
  ${KDE4_INCLUDES}
  ${QJSON_INCLUDE_DIR}
)

add_subdirectory(libplurk)
add_subdirectory(resource)

macro_display_feature_log()
