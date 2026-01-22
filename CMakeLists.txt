# CMakeList.txt : CMake project for CMakeProjectGRAP1, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.10)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project("CMakeProjectGRAP1")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Set directory of third party libraries
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty CACHE PATH "Base Directory of libs" FORCE)

# Get GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
)

# Disable building GLFW's extra tools to save time
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Set GLFW for this project
FetchContent_MakeAvailable(glfw)

# Get Glad v2
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.6
    SOURCE_SUBDIR cmake
)

# Set glad for this project
FetchContent_MakeAvailable(glad)

# Generate Glad library for OpenGL 4.6 Core
glad_add_library(glad_gl_core_46 STATIC
    REPRODUCIBLE 
    LOADER
    API gl:core=4.6
)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

# OpenGL should be preinstalled
find_package(OpenGL REQUIRED)

# Add source to this project's executable.

add_executable(CMakeProjectGRAP1 "CMakeProjectGRAP1.cpp" "CMakeProjectGRAP1.h")


# Link the required libraries
target_link_libraries(CMakeProjectGRAP1
    glfw
    OpenGL::GL
    glad_gl_core_46
    glm::glm
)

<<<<<<< Updated upstream
=======
add_custom_command(
    TARGET CMakeProjectGRAP1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/3D"
        "$<TARGET_FILE_DIR:CMakeProjectGRAP1>/3D"
    COMMENT "Copying assets to build directory"
)

add_custom_command(
    TARGET CMakeProjectGRAP1
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Shaders"
        "$<TARGET_FILE_DIR:CMakeProjectGRAP1>/Shaders"
    COMMENT "Copying assets to build directory"
)



>>>>>>> Stashed changes
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET CMakeProjectGRAP1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.