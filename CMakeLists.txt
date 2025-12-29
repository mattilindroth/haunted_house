cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HauntedHouse CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)


# Set Raylib paths (adjust if needed)
if(WIN32)
    set(raylib_DIR "C:/raylib/install/lib/cmake/raylib")
else()
    set(raylib_DIR "/usr/local/lib/cmake/raylib")
endif()


# set(CMAKE_PREFIX_PATH "C:/raylib/install" ${CMAKE_PREFIX_PATH})
if(WIN32)
    find_library(RAYLIB_LIBRARY NAMES raylib PATHS "C:/raylib/raylib/build/raylib")
    set(RAYLIB_INCLUDE_DIR "C:/raylib/install/include")
elseif(LINUX)
    find_library(RAYLIB_LIBRARY NAMES raylib PATHS "/usr/local/lib" "/usr/lib")
    set(RAYLIB_INCLUDE_DIR "/usr/local/include")
endif()


message(STATUS "Raylib library found at: ${RAYLIB_INCLUDE_DIR}")

# Diagnostics: print resolved variables for troubleshooting
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "RAYLIB_LIBRARY = ${RAYLIB_LIBRARY}")
message(STATUS "RAYLIB_INCLUDE_DIR = ${RAYLIB_INCLUDE_DIR}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

# Check whether raylib.h exists at the include dir we set
if(EXISTS "${RAYLIB_INCLUDE_DIR}/raylib.h")
    message(STATUS "Found raylib.h in ${RAYLIB_INCLUDE_DIR}")
else()
    message(WARNING "Could not find raylib.h at ${RAYLIB_INCLUDE_DIR}")
endif()

# Create an executable from the source file
add_executable(HauntedHouse 
            src/main.cpp
            src/falling_object.cpp
            src/ghost.cpp
            src/player.cpp
            src/splash.cpp
            src/sprite.cpp
)

# Include Raylib headers (target-based)
target_include_directories(HauntedHouse PRIVATE ${RAYLIB_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/include")

# Link Raylib static library (change to raylib.so if using shared)
if(WIN32)
    # On Windows, link raylib and required system libs
    target_link_libraries(HauntedHouse PRIVATE ${RAYLIB_LIBRARY} winmm opengl32 gdi32)
else()
    target_link_libraries(HauntedHouse PRIVATE ${RAYLIB_LIBRARY})
endif()

# Set output directories for build
set_target_properties(HauntedHouse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
