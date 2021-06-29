cmake_minimum_required(VERSION 3.16.3)
project (seagull VERSION 0.0.1)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (GCC)
    add_compile_options(-g -Wall -Wformat-security -Wstrict-overflow -Wsign-compare -Wempty-body -Wignored-qualifiers -Wuninitialized -Wtype-limits -Woverride-init -Wno-multichar -Wno-attributes -Wnull-dereference)
elseif (MSVC)
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

file(GLOB PROJECT_SOURCES "seagull/src/*.c")
file(GLOB PROJECT_HEADERS "seagull/headers/*.h")


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(${PROJECT_NAME} glfw glew)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
