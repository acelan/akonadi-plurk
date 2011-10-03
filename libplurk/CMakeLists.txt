project(plurk)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  #${KDE4_INCLUDES}
  #${QT_INCLUDES}
  #${QJSON_INCLUDE_DIR}
  ${QCA2_INCLUDE_DIR}
  ${QTOAUTH_INCLUDE_DIR}
)

set(plurk_SRCS
  plurkapi.cpp
  plurkapialerts.cpp
  plurkapiblocks.cpp
  plurkapicliques.cpp
  plurkapiemoticons.cpp
  plurkapifriendsandfans.cpp
  plurkapioauth.cpp
  plurkapiplurksearch.cpp
  plurkapiplurktop.cpp
  plurkapiprofile.cpp
  plurkapirealtime.cpp
  plurkapiresponses.cpp
  plurkapitimeline.cpp
  plurkapiusers.cpp
)

# kde4_add_kcfg_files(plurk_SRCS
# plurkapisettings.kcfgc
# )

kde4_add_library(plurk SHARED ${plurk_SRCS})

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

target_link_libraries(plurk
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KDEWEBKIT_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDEPIMLIBS_KABC_LIBS}
  ${KDEPIMLIBS_KPIMUTILS_LIBS}
  ${KDEPIMLIBS_KMIME_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QTOAUTH_LIBRARY}
  ${QJSON_LIBRARIES}
  ${QJSON_LIBRARY}
)

if(KDEPIM_44_COMPAT)
  target_link_libraries(plurk ${KDEPIMLIBS_KCAL_LIBS})
else(KDEPIM_44_COMPAT)
  target_link_libraries(plurk ${KDEPIMLIBS_KCALCORE_LIBS})
endif(KDEPIM_44_COMPAT)

set_target_properties(plurk PROPERTIES
  VERSION ${LIB_VERSION}
  SOVERSION ${LIB_SOVERSION}
)

install(TARGETS plurk ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  libplurk_export.h
  plurkapi.h
  plurkapialerts.h
  plurkapiblocks.h
  plurkapicliques.h
  plurkapiemoticons.h
  plurkapifriendsandfans.h
  plurkapioauth.h
  plurkapiplurksearch.h
  plurkapiplurktop.h
  plurkapiprofile.h
  plurkapirealtime.h
  plurkapiresponses.h
  plurkapitimeline.h
  plurkapiusers.h
  plurktypes.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/plurk COMPONENT Devel
)
