
########### next target ###############

set(fonttest_SRCS
   main.cpp
)

include_directories(${SDL2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS}
    ${IL_INCLUDE_DIR})

add_executable(fonttest ${fonttest_SRCS})

target_link_libraries(fonttest salt zeitgeist oxygen kerosin ${SDL2_LIBRARIES})

