cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HauntedMuseum)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set Raylib paths (adjust if needed)
#set(RAYLIB_DIR "$ENV{HOME}/source/raylib/build/raylib")
#set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
#set(RAYLIB_LIB_DIR "${RAYLIB_DIR}")
if(WIN32)
    find_library(RAYLIB_LIBRARY NAMES raylib PATHS "$ENV{HOME}/source/raylib/build/raylib")
elseif(LINUX)
    find_library(RAYLIB_LIBRARY NAMES raylib PATHS "/usr/local/lib" "/usr/lib")
endif()

# Include Raylib headers
include_directories(${RAYLIB_INCLUDE_DIR} 
                    "${PROJECT_SOURCE_DIR}/include"
)

# Create an executable from the source file
add_executable(HauntedMuseum 
            src/main.cpp
            src/falling_object.cpp
            src/player.cpp
            src/splash.cpp
            src/sprite.cpp
)

# Link Raylib static library (change to raylib.so if using shared)
target_link_libraries(HauntedMuseum ${RAYLIB_LIBRARY})

# Set output directories for build
set_target_properties(HauntedMuseum PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
