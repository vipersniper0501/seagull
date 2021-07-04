cmake_minimum_required(VERSION 3.16.3)
project (seagull VERSION 0.0.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-g -Wall -Wformat-security -Wstrict-overflow -Wshift-overflow)
if (MSVC)
    add_compile_options(/W4 /WX)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glew/build/cmake)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(seagull/vendor/glfw)

#Bring the headers into the project
include_directories(
    ${CMAKE_SOURCE_DIR}/seagull/headers
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glew/include)
if (APPLE)
    include_directories(
        /usr/local/opt/python@3.9/Frameworks/Python.framework/Versions/3.9/Headers)
else()
    find_package(Python COMPONENTS Interpreter Development)
endif()

file(GLOB PROJECT_SOURCES "seagull/src/*.c")
file(GLOB PROJECT_HEADERS "seagull/headers/*.h")
file(GLOB PROJECT_PYTHON_SOURCES "seagull/src/*.py")


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_PYTHON_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw glew)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} /usr/local/opt/python@3.9/Frameworks/Python.framework/Versions/3.9/lib/python3.9/config-3.9-darwin/libpython3.9.a)
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
