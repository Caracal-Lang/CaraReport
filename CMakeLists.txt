cmake_minimum_required(VERSION 3.30)

project(CaraReport LANGUAGES CXX VERSION 2026.3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

option(CARAREPORT_BUILD_SHARED "Build CaraReport as a shared library" OFF)
option(CARAREPORT_BUILD_TESTS "Enable building tests." OFF)
option(CARAREPORT_BUILD_EXAMPLE "Enable building the example." OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(CARAREPORT_BUILD_TESTS)
  include(CTest)
endif()


add_subdirectory(Source)

if(CARAREPORT_BUILD_TESTS)
    include(CaraTest)
    add_subdirectory(Tests)
endif()

if(CARAREPORT_BUILD_EXAMPLE)
    add_subdirectory(Examples)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/carareport.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/carareport.pc
  @ONLY
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/carareport.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/carareport-config-version.cmake"
    VERSION ${CARAREPORT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/carareport-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CaraReport"
)

