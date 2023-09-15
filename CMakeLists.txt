cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TextureLoader VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{SAPPHIRE_SDK})
    message(FATAL_ERROR "Unable to find Sapphire SDK! Please define SAPPHIRE_SDK environment variable to point to Sapphire")
else()
    message(STATUS "Found Sapphire: $ENV{SAPPHIRE_SDK}")
endif()

add_subdirectory($ENV{SAPPHIRE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/sapphire)

# Set up dependencies, resources, link Sapphire
setup_sapphire_mod(${PROJECT_NAME})