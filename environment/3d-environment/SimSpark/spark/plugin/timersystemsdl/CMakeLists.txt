########### next target ###############

set(timersystemsdl_LIB_SRCS
   export.cpp
   timersystemsdl.cpp
   timersystemsdl_c.cpp
   timersystemsdl.h
)

include_directories(${SDL2_INCLUDE_DIRS})
add_library(timersystemsdl MODULE ${timersystemsdl_LIB_SRCS})

target_link_libraries(timersystemsdl m ${spark_libs} ${SDL2_LIBRARIES})

if (NOT APPLE)
   set_target_properties(timersystemsdl PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS timersystemsdl DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
