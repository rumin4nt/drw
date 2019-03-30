cmake_minimum_required(VERSION 3.0)

project(drw)
set(drw_VERSION_MAJOR 0)
set(drw_VERSION_MINOR 0)
set(drw_VERSION_PATCH 1)


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(OperatingSystem "macos")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(OperatingSystem "linux")
endif()

if(WIN32)
        set(OperatingSystem "windows")
endif()

file(GLOB sources drw.c src/*.c src/*.cpp
src/type/drw_type.c
src/type/drw_type_hpvec.c
src/type/drw_type_asteroids.c
)

if(${OperatingSystem} MATCHES "macos")
	file(GLOB sources ${sources}
	src/type/drw_type_ftgl.c
	src/ext/*.c
	src/hacks/*.c
)

endif()

link_directories(${CMAKE_BINARY_DIR}
                 ${CMAKE_SOURCE_DIR}/contrib/platform/darwin /usr/local/lib)

add_library(drw STATIC ${sources})

option(BuildLibrary "Build the drw library for embedding or linking" ON)
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
