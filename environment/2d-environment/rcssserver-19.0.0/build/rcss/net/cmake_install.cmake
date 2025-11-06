# Install script for directory: /Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net

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
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/net/librcssnet.1.0.1.dylib"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/net/librcssnet.1.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librcssnet.1.0.1.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/librcssnet.1.dylib"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/net/librcssnet.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rcss/net" TYPE FILE FILES
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/addr.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/socket.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/udpsocket.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/tcpsocket.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/socketstreambuf.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/isocketstream.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/osocketstream.hpp"
    "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/rcss/net/iosocketstream.hpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/nathan.sidib/Code/VS_Code/rcsoccersim/rcssserver-19.0.0/build/rcss/net/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
