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

# Include Raylib headers
include_directories(${RAYLIB_INCLUDE_DIR} 
                    "${PROJECT_SOURCE_DIR}/include"
)

# Create an executable from the source file
add_executable(HauntedHouse 
            src/main.cpp
            src/falling_object.cpp
            src/player.cpp
            src/splash.cpp
            src/sprite.cpp
)

# Link Raylib static library (change to raylib.so if using shared)
if(WIN32)
    target_link_libraries(HauntedHouse PRIVATE ${RAYLIB_LIBRARY} winmm)
else()
    target_link_libraries(HauntedHouse PRIVATE ${RAYLIB_LIBRARY})
endif()

# Set output directories for build
set_target_properties(HauntedHouse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
