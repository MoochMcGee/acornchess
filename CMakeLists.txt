cmake_minimum_required (VERSION 3.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (acornchess)

#properly disable insource building, as CMAKE_DISABLE_IN_SOURCE_BUILD never works for me
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "Insource building is disabled.")
endif()

set(SOURCES
src/common.h
src/i808x_cpu.cpp
src/i808x_cpu.h
src/ibm5150.cpp
src/ibm5150.h
src/main.cpp
)

add_executable(acornchess ${SOURCES})