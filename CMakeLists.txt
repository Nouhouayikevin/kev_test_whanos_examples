cmake_minimum_required(VERSION 3.16)
project(rtype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-platform configuration
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Conan integration
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Try to find Conan-generated files
find_package(sfml QUIET)
find_package(Boost QUIET)

# Add subdirectories
add_subdirectory(Server)
add_subdirectory(Client)

# Custom targets for convenience
add_custom_target(server
    DEPENDS rtype_server
    COMMENT "Building R-Type server"
)

add_custom_target(client
    DEPENDS rtype_client
    COMMENT "Building R-Type client"
)

add_custom_target(all_rtype
    DEPENDS rtype_server rtype_client
    COMMENT "Building both R-Type server and client"
)

# Install rules
install(TARGETS rtype_server DESTINATION bin)
install(TARGETS rtype_client DESTINATION bin)
install(DIRECTORY Assets/ DESTINATION share/rtype/assets)