cmake_minimum_required(VERSION 3.16)
project(rtype_server LANGUAGES CXX)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/Server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/GameEngine_Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Include/All)

# Add shared sources (scenes, sfml wrappers, managers) used by both client and server
file(GLOB_RECURSE SFML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/sfml/*.cpp)
file(GLOB_RECURSE SCENE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/scenes/*.cpp)
file(GLOB_RECURSE MANAGER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../All_src/managers/*.cpp)
# Also include client application helper used by MenuScene to switch modes
file(GLOB CLIENT_APPLICATION_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/src/ClientApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/src/OfflineApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/src/OnlineApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/src/NetworkClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Client/src/Protocole.cpp
)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if(ASIO_INCLUDE_DIR)
    include_directories(${ASIO_INCLUDE_DIR})
endif()

# Collect all server source files
file(GLOB_RECURSE SERVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# GameEngine source files
file(GLOB_RECURSE GAMEENGINE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameEngine/Core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameEngine/managers/*.cpp)

# Create executable
add_executable(rtype_server ${SERVER_SOURCES} ${SFML_SOURCES} ${SCENE_SOURCES} ${MANAGER_SOURCES} ${CLIENT_APPLICATION_SOURCES} ${GAMEENGINE_SOURCES})

# Link libraries
if(Boost_FOUND)
    target_link_libraries(rtype_server PRIVATE Boost::system)
endif()

# Link SFML for server if available (needed for ServerOfflineApplication which uses SFML wrappers)
find_package(sfml QUIET)
if(NOT sfml_FOUND)
    # Fallback to pkg-config like the client does
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SFML sfml-all>=2.5)
    endif()
endif()

if(sfml_FOUND OR SFML_FOUND)
    if(sfml_FOUND)
        target_link_libraries(rtype_server PRIVATE 
            sfml::sfml-graphics 
            sfml::sfml-window 
            sfml::sfml-system 
            sfml::sfml-audio
            sfml::sfml-network
        )
    else()
        target_link_libraries(rtype_server PRIVATE ${SFML_LIBRARIES})
        target_include_directories(rtype_server PRIVATE ${SFML_INCLUDE_DIRS})
        target_compile_options(rtype_server PRIVATE ${SFML_CFLAGS_OTHER})
    endif()
else()
    message(WARNING "SFML not found for server — server offline features that use SFML may fail to link.")
endif()

# Threading
find_package(Threads REQUIRED)
target_link_libraries(rtype_server PRIVATE Threads::Threads)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(rtype_server PRIVATE ws2_32 wsock32)
endif()
