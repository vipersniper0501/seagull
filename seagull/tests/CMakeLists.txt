cmake_minimum_required(VERSION 3.16.3)

message(STATUS "Building Tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set(BUILD_UTILS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glew/build/cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glm)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glfw)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_WERROR OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
if (MSVC)
    set(ASSIMP_INSTALL_PDB ON)
else()
    set(ASSIMP_INSTALL_PDB OFF)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/assimp)


# Create root_directory file containing project root directory for use in engine
configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# Bring the headers and sources into the project
include_directories(
    ${CMAKE_SOURCE_DIR}/seagull/headers
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glew/include
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glm
    ${CMAKE_SOURCE_DIR}/seagull/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/seagull/vendor/stbimage
    ${CMAKE_SOURCE_DIR}/seagull/vendor/imgui)

file(GLOB PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/TestPinwheel.cpp")
file(GLOB PROJECT_HEADERS "../headers/*.h")

file(GLOB VENDOR_IMGUI_S "../vendor/imgui/*.cpp")
file(GLOB VENDOR_IMGUI_H "../vendor/imgui/*.h")

set(VENDOR_IMGUI_OPENGL_GLFW
    "../vendor/imgui/backends/imgui_impl_glfw.h"
    "../vendor/imgui/backends/imgui_impl_glfw.cpp"
    "../vendor/imgui/backends/imgui_impl_opengl3.h"
    "../vendor/imgui/backends/imgui_impl_opengl3.cpp")


project(PinwheelTest)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
    ${VENDOR_IMGUI_S} ${VENDOR_IMGUI_H} ${VENDOR_IMGUI_OPENGL_GLFW})

# Start linking libraries
target_link_libraries(${PROJECT_NAME} glfw glew glm assimp)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
elseif (UNIX)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)

    if (OPENGL_FOUND)
        message(STATUS "Looking for OpenGL - FOUND")
    else()
        message(STATUS "Looking for OpenGL - NOT FOUND")
    endif()

    # checks if the unix system is Windows Subsystem for Linux (WSL)
    if (CMAKE_SYSTEM MATCHES "microsoft-standard-WSL.$")
        set(OPENGL_gl_LIBRARY "/usr/lib/x86_64-linux-gnu/")
        set(OPENGL_glu_LIBRARY "/usr/lib/x86_64-linux-gnu/")
        set(OPENGL_glx_LIBRARY "/usr/lib/x86_64-linux-gnu/")
        message(STATUS "WSL: OpenGL Library Location: ${OPENGL_LIBRARIES}")
        message(STATUS "WSL: OpenGL Header Location: ${OPENGL_INCLUDE_DIR}")
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    elseif (OPENGL_OPENGL_FOUND)
        message(STATUS "Unix: Looking for OpenGL Library - FOUND")
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    else()
        message(STATUS "Unix: Looking for OpenGL Library - NOT FOUND")
    endif()
elseif (WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()

# Copy windows dll files to where the binary is located
if (WIN32 AND MINGW)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/bin/glew32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/seagull/vendor/glfw/src/glfw3.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/seagull/vendor/assimp/bin/libassimp.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
elseif(WIN32 AND MSVC_IDE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/seagull/vendor/glfw/src/$<CONFIGURATION>" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/seagull/vendor/assimp/bin/$<CONFIGURATION>" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# Set output directory for binary
# The visual studio IDE has its own place where it puts the executable. Would rather
# not mess with something that is already working
if (NOT MSVC_IDE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()

