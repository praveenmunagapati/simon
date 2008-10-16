include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonappcore_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/generalsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simoncontrol.cpp
)

set( simonappcore_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/simoncontrol.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonappcore_export.h
)

kde4_add_ui_files(simonappcore_LIB_SRCS generalsettingsdlg.ui)
kde4_add_kcfg_files(simonappcore_LIB_SRCS coreconfiguration.kcfgc)

kde4_add_library( simonappcore  SHARED ${simonappcore_LIB_SRCS} )
target_link_libraries( simonappcore ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
simoninfo simonlogging speechgui simonrecognitioncontrol simonactions)


set_target_properties( simonappcore
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonappcore_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/AppCore
  COMPONENT Devel
)
 
install( TARGETS simonappcore  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simongeneralconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )