#####################
## FindPoly.cmake
## Top contributors (to current version):
##   Gereon Kremer
## This file is part of the CVC4 project.
## Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
## in the top-level source directory and their institutional affiliations.
## All rights reserved.  See the file COPYING in the top-level source
## directory for licensing information.
##
# Find LibPoly
# Poly_FOUND - should always be true
# Poly - target for the libpoly library
# Polyxx - target for the C++ interface of libpoly, also links Poly

include(deps-helper)

find_path(Poly_INCLUDE_DIR NAMES poly/poly.h)
find_library(Poly_LIBRARIES NAMES poly)
find_library(PolyXX_LIBRARIES NAMES polyxx)

set(Dummy_FOUND_SYSTEM FALSE)
if(Poly_INCLUDE_DIR
   AND Poly_LIBRARIES
   AND PolyXX_LIBRARIES
)
  set(Poly_FOUND_SYSTEM TRUE)

  file(STRINGS ${Poly_INCLUDE_DIR}/poly/version.h Poly_VERSION
       REGEX "^#define[\t ]+LIBPOLY_VERSION [0-9+]+"
  )
  string(REGEX MATCH "[0-9.]+" Poly_VERSION "${Poly_VERSION}")

  check_system_version("Poly")
endif()

if(NOT Poly_FOUND_SYSTEM)
  include(ExternalProject)

  # TODO(#4706): Use proper release, after the next release
  set(Poly_VERSION "bae67639726f63ed508a30845108bfdac4a77546")

  check_if_cross_compiling(CCWIN "Windows" "")
  if(CCWIN)
    # Roughly following https://stackoverflow.com/a/44383330/2375725
    set(patchcmd
        PATCH_COMMAND
        patch
        <SOURCE_DIR>/src/CMakeLists.txt
        ${CMAKE_CURRENT_LIST_DIR}/deps-utils/Poly-patch-cmake.patch
        # Avoid %z and %llu format specifiers
        COMMAND find <SOURCE_DIR>/ -type f -exec
                sed -i.orig "s/%z[diu]/%\" PRIu64 \"/g" {} +
        COMMAND find <SOURCE_DIR>/ -type f -exec
                sed -i.orig "s/%ll[du]/%\" PRIu64 \"/g" {} +
        # Make sure the new macros are available
        COMMAND find <SOURCE_DIR>/ -type f -exec
                sed -i.orig "s/#include <stdio.h>/#include <stdio.h>\\n#include <inttypes.h>/" {} +
        COMMAND find <SOURCE_DIR>/ -type f -exec
                sed -i.orig "s/#include <cstdio>/#include <cstdio>\\n#include <inttypes.h>/" {} +
    )
  else()
    unset(patchcmd)
  endif()

  ExternalProject_Add(
    Poly-EP
    PREFIX ${DEPS_PREFIX}
    URL https://github.com/SRI-CSL/libpoly/archive/${Poly_VERSION}.tar.gz
    URL_HASH SHA1=2e79d5220d3ecbb40811463fcf12c5ddbd4b9f30
    DOWNLOAD_NAME libpoly.tar.gz
    ${patchcmd}
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
               -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
               -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
               -DLIBPOLY_BUILD_PYTHON_API=OFF
               -DLIBPOLY_BUILD_STATIC=ON
               -DLIBPOLY_BUILD_STATIC_PIC=ON
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} static_pic_poly static_pic_polyxx
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
    COMMAND ${CMAKE_COMMAND} -E copy src/libpicpoly.a
            <INSTALL_DIR>/lib/libpicpoly.a
    COMMAND ${CMAKE_COMMAND} -E copy src/libpicpolyxx.a
            <INSTALL_DIR>/lib/libpicpolyxx.a
    BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libpicpoly.a
                     <INSTALL_DIR>/lib/libpicpolyxx.a
  )
  ExternalProject_Add_Step(
    Poly-EP cleanup
    DEPENDEES install
    COMMAND ${CMAKE_COMMAND} -E remove_directory <SOURCE_DIR>/test/
    COMMAND ${CMAKE_COMMAND} -E remove_directory <BINARY_DIR>/test/
  )
  add_dependencies(Poly-EP GMP)

  set(Poly_INCLUDE_DIR "${DEPS_BASE}/include/")
  set(Poly_LIBRARIES "${DEPS_BASE}/lib/libpicpoly.a")
  set(PolyXX_LIBRARIES "${DEPS_BASE}/lib/libpicpolyxx.a")
endif()

set(Poly_FOUND TRUE)

add_library(Poly STATIC IMPORTED GLOBAL)
set_target_properties(Poly PROPERTIES IMPORTED_LOCATION "${Poly_LIBRARIES}")
set_target_properties(
  Poly PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Poly_INCLUDE_DIR}"
)

add_library(Polyxx STATIC IMPORTED GLOBAL)
set_target_properties(Polyxx PROPERTIES IMPORTED_LOCATION "${PolyXX_LIBRARIES}")
set_target_properties(
  Polyxx PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Poly_INCLUDE_DIR}"
)
set_target_properties(Polyxx PROPERTIES INTERFACE_LINK_LIBRARIES Poly)

mark_as_advanced(Poly_FOUND)
mark_as_advanced(Poly_FOUND_SYSTEM)
mark_as_advanced(Poly_INCLUDE_DIR)
mark_as_advanced(Poly_LIBRARIES)
mark_as_advanced(PolyXX_LIBRARIES)

if(Poly_FOUND_SYSTEM)
  message(STATUS "Found Poly ${Poly_VERSION}: ${Poly_LIBRARIES}")
else()
  message(STATUS "Building Poly ${Poly_VERSION}: ${Poly_LIBRARIES}")
  add_dependencies(Poly Poly-EP)
  add_dependencies(Polyxx Poly-EP)
endif()
