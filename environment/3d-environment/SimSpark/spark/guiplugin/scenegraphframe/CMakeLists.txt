########### target files ###############
set(scenegraphframe_QOBJECT_HEADERS
	scenegraphframe.h
	scenegraphframe_propertyframe.h
	scenegraphframe_scenegraphwidget.h
)

set(scenegraphframe_HEADERS
	export.h
)

set(scenegraphframe_SOURCES
	scenegraphframe.cpp
	scenegraphframe_propertyframe.cpp
	scenegraphframe_scenegraphwidget.cpp
)

set(scenegraphframe_FORMS
	scenegraphframe.ui
	scenegraphframe_propertyframe.ui
	scenegraphframe_scenegraphwidget.ui
)

#set(scenegraphframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
qt5_wrap_cpp(scenegraphframe_QOBJECT_HEADERS_MOC ${scenegraphframe_QOBJECT_HEADERS})
qt5_wrap_ui(scenegraphframe_FORMS_HEADERS ${scenegraphframe_FORMS})
#qt5_add_resources(scenegraphframe_RESOURCES_RCC ${scenegraphframe_RESOURCES})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/carbon)

add_library(
  scenegraphframe MODULE 
  ${scenegraphframe_SOURCES} 
  ${scenegraphframe_QOBJECT_HEADERS}
  ${scenegraphframe_QOBJECT_HEADERS_MOC}
  ${scenegraphframe_HEADERS}
  ${scenegraphframe_FORMS_HEADERS})
  #${scenegraphframe_RESOURCES_RCC})

target_link_libraries(
  scenegraphframe
  ${OPENGL_LIBRARIES} 
  ${Qt5Widgets_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(scenegraphframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS scenegraphframe DESTINATION ${CARBON_PLUGIN_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_PLUGIN_INCLUDE_DIR}/scenegraphframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
