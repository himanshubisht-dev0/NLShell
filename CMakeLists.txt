cmake_minimum_required(VERSION 3.10)
project(IntelligentShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Check for readline library more robustly
find_library(READLINE_LIB readline)
find_path(READLINE_INCLUDE_DIR readline/readline.h)

if(READLINE_LIB AND READLINE_INCLUDE_DIR)
    message(STATUS "Found readline library: ${READLINE_LIB}")
    message(STATUS "Found readline headers: ${READLINE_INCLUDE_DIR}")
else()
    # Try pkg-config as fallback
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(READLINE readline)
        if(READLINE_FOUND)
            message(STATUS "Found readline via pkg-config")
        else()
            message(FATAL_ERROR "readline library not found. Install with: sudo apt-get install libreadline-dev")
        endif()
    else()
        message(FATAL_ERROR "readline library not found. Install with: sudo apt-get install libreadline-dev")
    endif()
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/shell.cpp
    src/parser.cpp
    src/exec.cpp
    src/bridge.cpp
)

# Create executable
add_executable(intellishell ${SOURCES})

# Link libraries
if(READLINE_LIB)
    target_link_libraries(intellishell ${READLINE_LIB})
elseif(READLINE_LIBRARIES)
    target_link_libraries(intellishell ${READLINE_LIBRARIES})
endif()

# Add include directories
if(READLINE_INCLUDE_DIR)
    target_include_directories(intellishell PRIVATE ${READLINE_INCLUDE_DIR})
elseif(READLINE_INCLUDE_DIRS)
    target_include_directories(intellishell PRIVATE ${READLINE_INCLUDE_DIRS})
endif()
