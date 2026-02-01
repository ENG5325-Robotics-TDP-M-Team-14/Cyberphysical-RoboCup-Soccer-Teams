########### target files ###############
set(simulationframe_QOBJECT_HEADERS
	simulationframe.h
	simulationframe_taskwidget.h
)

set(simulationframe_HEADERS
	export.h
)

set(simulationframe_SOURCES
	simulationframe.cpp
	simulationframe_taskwidget.cpp
)

set(simulationframe_FORMS
	simulationframe.ui
	simulationframe_taskwidget.ui
)

#set(simulationframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
qt5_wrap_cpp(simulationframe_QOBJECT_HEADERS_MOC ${simulationframe_QOBJECT_HEADERS})
qt5_wrap_ui(simulationframe_FORMS_HEADERS ${simulationframe_FORMS})
#qt5_add_resources(simulationframe_RESOURCES_RCC ${simulationframe_RESOURCES})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/carbon)

add_library(
  simulationframe MODULE 
  ${simulationframe_SOURCES} 
  ${simulationframe_QOBJECT_HEADERS}
  ${simulationframe_QOBJECT_HEADERS_MOC}
  ${simulationframe_HEADERS}
  ${simulationframe_FORMS_HEADERS})
  #${simulationframe_RESOURCES_RCC})

target_link_libraries(
  simulationframe
  ${OPENGL_LIBRARIES} 
  ${Qt5Widgets_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(simulationframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS simulationframe DESTINATION ${CARBON_PLUGIN_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_PLUGIN_INCLUDE_DIR}/simulationframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
