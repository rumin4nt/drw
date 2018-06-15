cmake_minimum_required(VERSION 3.0)

project(drw)
set(drw_VERSION_MAJOR 0)
set(drw_VERSION_MINOR 0)
set(drw_VERSION_PATCH 1)

file(GLOB_RECURSE sources drw.c src/*.c)

link_directories(${CMAKE_BINARY_DIR}
                 ${CMAKE_SOURCE_DIR}/contrib/platform/darwin /usr/local/lib)

add_library(drw STATIC ${sources})

option(BuildLibrary "Build the brush library for embedding or linking" ON)
option(BuildDrwTest "BuildTest" OFF)
option(BuildDocumentation "BuildDocumentation" OFF)

target_include_directories(drw
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/..
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/deps
                           )

if(${BuildDrwTest})
  add_subdirectory("test")
endif()

set_property(TARGET drw PROPERTY C_STANDARD 99)

# target_link_libraries(drw LINK_PUBLIC glut)
