cmake_minimum_required(VERSION 3.16)
project(rtype_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Platform-specific compiler flags
if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -g3)
endif()

# Find SFML (prioritize Conan packages)
find_package(sfml QUIET)
if(NOT sfml_FOUND)
    # Fallback to pkg-config for system installations
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SFML sfml-all>=2.5)
    endif()
    if(NOT SFML_FOUND)
        message(FATAL_ERROR "SFML not found. Please install SFML or use Conan.")
    endif()
endif()

# Networking library detection (prioritize Conan packages)
find_package(Boost QUIET COMPONENTS system)
if(Boost_FOUND)
    message(STATUS "Using Boost.Asio for networking")
    add_compile_definitions(RTYPE_USE_BOOST_ASIO)
else()
    # Fallback to system-installed packages
    find_path(ASIO_INCLUDE_DIR asio.hpp PATH_SUFFIXES asio include/asio)
    if(ASIO_INCLUDE_DIR)
        message(STATUS "Found standalone Asio: ${ASIO_INCLUDE_DIR}")
        add_compile_definitions(RTYPE_USE_STANDALONE_ASIO)
    else()
        message(FATAL_ERROR "No networking library found. Please install Boost or standalone Asio.")
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/All)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/Client_include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/GameEngine_Include)

if(ASIO_INCLUDE_DIR)
    include_directories(${ASIO_INCLUDE_DIR})
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Client source files
file(GLOB_RECURSE CLIENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# SFML wrapper source files
file(GLOB_RECURSE SFML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/sfml/*.cpp)

# Scene and manager source files
file(GLOB_RECURSE SCENE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/scenes/*.cpp)
file(GLOB_RECURSE MANAGER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/managers/*.cpp)
# Also include server helpers needed by scenes
file(GLOB SERVER_HELPERS ${CMAKE_CURRENT_SOURCE_DIR}/../Server/src/SnapshotHelper.cpp)

# GameEngine source files
file(GLOB_RECURSE GAMEENGINE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameEngine/Core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameEngine/managers/*.cpp)

# Create executable
add_executable(rtype_client 
    ${CLIENT_SOURCES}
    ${SFML_SOURCES}
    ${SCENE_SOURCES}
    ${MANAGER_SOURCES}
    ${SERVER_HELPERS}
    ${GAMEENGINE_SOURCES}
)

# Link libraries
if(sfml_FOUND)
    # Conan SFML
    target_link_libraries(rtype_client PRIVATE 
        sfml::sfml-graphics 
        sfml::sfml-window 
        sfml::sfml-system 
        sfml::sfml-audio
        sfml::sfml-network
    )
elseif(SFML_FOUND)
    # System SFML via pkg-config
    target_link_libraries(rtype_client PRIVATE ${SFML_LIBRARIES})
    target_include_directories(rtype_client PRIVATE ${SFML_INCLUDE_DIRS})
    target_compile_options(rtype_client PRIVATE ${SFML_CFLAGS_OTHER})
endif()

# Networking libraries
if(Boost_FOUND)
    target_link_libraries(rtype_client PRIVATE Boost::system)
endif()

# Threading
find_package(Threads REQUIRED)
target_link_libraries(rtype_client PRIVATE Threads::Threads)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(rtype_client PRIVATE ws2_32 wsock32)
endif()

# Copy assets directory to build directory
add_custom_command(TARGET rtype_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets
    ${CMAKE_CURRENT_BINARY_DIR}/Assets
)