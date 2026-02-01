########### target files ###############
set(guigridlayout_QOBJECT_HEADERS
	guigridlayout.h
)

set(guigridlayout_HEADERS
	export.h
)

set(guigridlayout_SOURCES
	guigridlayout.cpp
)

set(guigridlayout_FORMS
	guigridlayout.ui
)

#set(guigridlayout_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
qt5_wrap_cpp(guigridlayout_QOBJECT_HEADERS_MOC ${guigridlayout_QOBJECT_HEADERS})
qt5_wrap_ui(guigridlayout_FORMS_HEADERS ${guigridlayout_FORMS})
#qt5_add_resources(guigridlayout_RESOURCES_RCC ${guigridlayout_RESOURCES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_SOURCE_DIR}/carbon 
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

add_library(
  guigridlayout MODULE 
  ${guigridlayout_SOURCES}
  ${guigridlayout_QOBJECT_HEADERS}  
  ${guigridlayout_QOBJECT_HEADERS_MOC}
  ${guigridlayout_HEADERS}
  ${guigridlayout_FORMS_HEADERS})
  #${guigridlayout_RESOURCES_RCC})

target_link_libraries(
  guigridlayout
  ${OPENGL_LIBRARIES} 
  ${Qt5Widgets_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(guigridlayout PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS guigridlayout DESTINATION ${CARBON_LAYOUT_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_LAYOUT_INCLUDE_DIR}/guigridlayout
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
