# Install script for directory: /Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Libraries" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/librcssclangparser.18.0.0.dylib"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/librcssclangparser.18.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librcssclangparser.18.0.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librcssclangparser.18.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Libraries" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/librcssclangparser.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rcss/clang" TYPE FILE FILES
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangparser.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_tok.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangbuilder.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangmsgbuilder.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangmsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangmetamsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangfreeformmsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangunsuppmsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangrulemsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangdelmsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clanginfomsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangadvicemsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangdefmsg.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangaction.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/clangutil.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/coach_lang_comp.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/coach_lang_parser.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/arithop.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/compop.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/cond.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/region.h"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/clang/rule.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/clang/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
