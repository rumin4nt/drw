cmake_minimum_required (VERSION 3.0)

project(drw)
set (drw_VERSION_MAJOR 0)
set (drw_VERSION_MINOR 0)
set (drw_VERSION_PATCH 1)

file(GLOB_RECURSE sources *.c src/*.c)

add_library(drw STATIC ${sources})

option(BuildLibrary "Build the brush library for embedding or linking" ON)
option(BuildTest "BuildTest" ON)
option(BuildDocumentation "BuildDocumentation" OFF)

if(${BuildTest})
  add_subdirectory("test")
endif()

set_property(TARGET drw PROPERTY C_STANDARD 99)

target_include_directories (drw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories (drw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/include)
target_include_directories (drw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib)
target_include_directories (drw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/deps)
