cmake_minimum_required(VERSION 2.8)
project(icex)

# Glob files (make sure to clear cmake cache and rerun after adding/removing files)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)
file(GLOB_RECURSE SHADERS shaders/*.vert shaders/*.frag shaders/*.comp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile with c++14
#set(CMAKE_CXX_STANDARD 11)
add_definitions('--std=c++11')

# Preprocessor macros to locate resources and shader directory
add_definitions(-DRESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources/")
add_definitions(-DSHADER_PATH="${CMAKE_SOURCE_DIR}/shaders/")

# Find all packages and check we found them
# If OpenCV and GLEW are not in default directories, set OpenCV_DIR and GLEW_DIR environment variables
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{OpenCV_DIR} $ENV{GLEW_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# set(GLFW_INCLUDE_DIR $ENV{GLFW_INCLUDE_DIR})# These should be set automatically by the GLFW Cmake file
# set(GLFW_LIBRARIES $ENV{GLFW_LIBRARIES})# These should be set automatically by the GLFW Cmake file

find_package(OpenGL REQUIRED)
find_package(GLEW)
find_path(EIGEN3_INCLUDE_DIR Eigen/Dense PATHS ENV EIGEN3_INCLUDE_DIR PATH_SUFFIXES eigen eigen3)
find_path(GLM_INCLUDE_DIR glm/glm.hpp PATHS ENV GLM_DIR GLM_LOCATION) 
find_package(OpenCV REQUIRED)


# Get the GLFW environment variable. There should be a CMakeLists.txt in the 
# specified directory.
set(GLFW_DIR "$ENV{GLFW_DIR}")
if(NOT GLFW_DIR)
  message(FATAL_ERROR "Please point the environment variable GLFW_DIR to the root directory of your GLFW3 installation.")
endif()
option(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
option(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
option(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
if(CMAKE_BUILD_TYPE MATCHES Release)
  add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/release)
else()
  add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/debug)
endif()
include_directories(${GLFW_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES})




if (NOT EIGEN3_INCLUDE_DIR)
  message(FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()

if (NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "Please point the environment variable GLM_DIR or GLM_LOCATION to the include directory of your GLM installation.")
endif()

if (NOT GLEW_FOUND)
    if (DEFINED ENV{GLEW_DIR})
        if (WIN32)
            set(GLFW_LIBRARY "$ENV{GLEW_DIR}/lib/Release/Win32/glew32s.lib")
        else()
            set(GLFW_LIBRARY "$ENV{GLEW_DIR}/lib/libGLEW.a")
        endif()
    else()
        message(FATAL_ERROR "Can't find GLEW")
    endif()
endif()


target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${OpenCV_LIBS}
)


# Include required directories and link libraries
include_directories(
    src/
    src/imgui
    ${EIGEN3_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${OpenCV_LIBS}
)

if (APPLE)
    add_definitions(-std=c++14)
endif()
