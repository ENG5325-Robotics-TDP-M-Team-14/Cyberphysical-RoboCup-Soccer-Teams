########### next target ###############

set(timersystemstd_LIB_SRCS
   export.cpp
   timersystemstd.cpp
   timersystemstd_c.cpp
   timersystemstd.h
)

add_library(timersystemstd MODULE ${timersystemstd_LIB_SRCS})

target_link_libraries(timersystemstd ${spark_libs})

if (NOT APPLE)
   set_target_properties(timersystemstd PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS timersystemstd DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
