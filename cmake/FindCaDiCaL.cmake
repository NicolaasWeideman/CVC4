#####################
## FindCaDiCaL.cmake
## Top contributors (to current version):
##   Mathias Preiner
## This file is part of the CVC4 project.
## Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
## in the top-level source directory and their institutional affiliations.
## All rights reserved.  See the file COPYING in the top-level source
## directory for licensing information.
##
# Find CaDiCaL
# CaDiCaL_FOUND - system has CaDiCaL lib
# CaDiCaL_INCLUDE_DIR - the CaDiCaL include directory
# CaDiCaL_LIBRARIES - Libraries needed to use CaDiCaL

include(deps-helper)

find_path(CaDiCaL_INCLUDE_DIR NAMES cadical.hpp)
find_library(CaDiCaL_LIBRARIES NAMES cadical)

set(CaDiCaL_FOUND_SYSTEM FALSE)
if(CaDiCaL_INCLUDE_DIR AND CaDiCaL_LIBRARIES)
  set(CaDiCaL_FOUND_SYSTEM TRUE)

  # Unfortunately it is not part of the headers
  find_library(CaDiCaL_BINARY NAMES cadical)
  if(CaDiCaL_BINARY)
    execute_process(
      COMMAND ${CaDiCaL_BINARY} --version OUTPUT_VARIALE CaDiCaL_VERSION
    )
  else()
    set(CaDiCaL_VERSION "")
  endif()

  check_system_version("CaDiCaL")
endif()

if(NOT CaDiCaL_FOUND_SYSTEM)
  include(CheckSymbolExists)
  include(ExternalProject)

  fail_if_include_missing("sys/resource.h" "CaDiCaL")

  set(CaDiCaL_VERSION "1.2.1")

  # avoid configure script and instantiate the makefile manually the configure
  # scripts unnecessarily fails for cross compilation thus we do the bare
  # minimum from the configure script here
  set(CXXFLAGS "-fPIC -O3 -DNDEBUG -DQUIET -std=c++11")
  # check for getc_unlocked
  check_symbol_exists("getc_unlocked" "cstdio" HAVE_UNLOCKED_IO)
  if(NOT HAVE_UNLOCKED_IO)
    set(CXXFLAGS "${CXXFLAGS} -DNUNLOCKED")
  endif()

  ExternalProject_Add(
    CaDiCaL-EP
    PREFIX ${DEPS_PREFIX}
    BUILD_IN_SOURCE ON
    URL https://github.com/arminbiere/cadical/archive/refs/tags/rel-${CaDiCaL_VERSION}.tar.gz
    URL_HASH SHA1=9de1176737b74440921ba86395fe5edbb3b131eb
    CONFIGURE_COMMAND mkdir -p <SOURCE_DIR>/build
    # avoid configure script, prepare the makefile manually
    COMMAND
      sed -e "s,@CXX@,${CMAKE_CXX_COMPILER}," -e "s,@CXXFLAGS@,${CXXFLAGS}," -e
      "s,@MAKEFLAGS@,," <SOURCE_DIR>/makefile.in > <SOURCE_DIR>/build/makefile
    # use $(MAKE) instead of "make" to allow for parallel builds
    BUILD_COMMAND $(MAKE) -C <SOURCE_DIR>/build libcadical.a
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/build/libcadical.a
                    <INSTALL_DIR>/lib/libcadical.a
    COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/src/cadical.hpp
            <INSTALL_DIR>/include/cadical.hpp
  )

  set(CaDiCaL_INCLUDE_DIR "${DEPS_BASE}/include/")
  set(CaDiCaL_LIBRARIES "${DEPS_BASE}/lib/libcadical.a")
endif()

set(CaDiCaL_FOUND TRUE)

add_library(CaDiCaL STATIC IMPORTED GLOBAL)
set_target_properties(
  CaDiCaL PROPERTIES IMPORTED_LOCATION "${CaDiCaL_LIBRARIES}"
)
set_target_properties(
  CaDiCaL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CaDiCaL_INCLUDE_DIR}"
)

mark_as_advanced(CaDiCaL_FOUND)
mark_as_advanced(CaDiCaL_FOUND_SYSTEM)
mark_as_advanced(CaDiCaL_INCLUDE_DIR)
mark_as_advanced(CaDiCaL_LIBRARIES)

if(CaDiCaL_FOUND_SYSTEM)
  message(STATUS "Found CaDiCaL ${CaDiCaL_VERSION}: ${CaDiCaL_LIBRARIES}")
else()
  message(STATUS "Building CaDiCaL ${CaDiCaL_VERSION}: ${CaDiCaL_LIBRARIES}")
  add_dependencies(CaDiCaL CaDiCaL-EP)
endif()
