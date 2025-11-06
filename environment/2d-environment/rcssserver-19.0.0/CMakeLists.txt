cmake_minimum_required(VERSION 3.20)

project(RCSSServer VERSION 19.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# --- Policies ---
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)   # Prefer FindBoost (module) over BoostConfig
endif()
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)   # Honor <Pkg>_ROOT (e.g., Boost_ROOT)
endif()

# --- Homebrew hints (macOS) ---
if(APPLE)
  if(EXISTS "/opt/homebrew/opt/boost")
    set(Boost_ROOT "/opt/homebrew/opt/boost")
    set(BOOST_ROOT  "/opt/homebrew/opt/boost")
  endif()
  if(EXISTS "/opt/homebrew/opt/bison/bin/bison")
    set(BISON_EXECUTABLE "/opt/homebrew/opt/bison/bin/bison" CACHE FILEPATH "" FORCE)
  endif()
  if(EXISTS "/opt/homebrew/opt/flex/bin/flex")
    set(FLEX_EXECUTABLE "/opt/homebrew/opt/flex/bin/flex" CACHE FILEPATH "" FORCE)
    # Flex headers live here on Homebrew
    include_directories(SYSTEM "/opt/homebrew/opt/flex/include")
  endif()
endif()

# Force module-mode for Boost; avoid Config packages entirely
set(Boost_NO_BOOST_CMAKE ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG OFF)

# --- Deps ---
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
  set(HAVE_LIBZ TRUE)
endif()

find_package(BISON 3.4 REQUIRED)
find_package(FLEX  2.6 REQUIRED)

# Boost: 'system' header-only since 1.70; stub removed in 1.89 -> make optional
find_package(Boost 1.44 REQUIRED OPTIONAL_COMPONENTS system filesystem)

# Provide a header-only compatibility target if Boost::system is absent
if(NOT TARGET Boost::system)
  add_library(Boost::system INTERFACE IMPORTED)
  target_include_directories(Boost::system INTERFACE ${Boost_INCLUDE_DIRS})
endif()

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

include(GNUInstallDirs)
include(CheckIncludeFileCXX)

check_include_file_cxx("sys/socket.h"  HAVE_SYS_SOCKET_H)
check_include_file_cxx("sys/param.h"   HAVE_SYS_PARAM_H)
check_include_file_cxx("sys/time.h"    HAVE_SYS_TIME_H)
check_include_file_cxx("netinet/in.h"  HAVE_NETINET_IN_H)
check_include_file_cxx("arpa/inet.h"   HAVE_ARPA_INET_H)
check_include_file_cxx("netdb.h"       HAVE_NETDB_H)
check_include_file_cxx("unistd.h"      HAVE_UNISTD_H)
check_include_file_cxx("poll.h"        HAVE_POLL_H)
check_include_file_cxx("pwd.h"         HAVE_PWD_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake config.h)

# Build as-is; scanners handle their own yyFlexLexer renames in the .l files
add_subdirectory(rcss)
add_subdirectory(src)
