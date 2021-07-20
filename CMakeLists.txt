cmake_minimum_required(VERSION 3.16.3)
project (seagull VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-g -Wall -Wformat-security -Wstrict-overflow -Wshift-overflow)
if (MSVC)
    add_compile_options(/W4 /WX)
endif()

IF(APPLE)
    add_compile_options(-fvisibility=default)
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

set(BUILD_SHARED_LIBS OFF)
set(BUILD_UTILS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glew/build/cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glm)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/glfw)

set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/seagull/vendor/assimp)
	
# set(CGLM_SHARED OFF)
# set(CGLM_STATIC ON)
# add_subdirectory(seagull/vendor/cglm)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

#Bring the headers into the project
include_directories(
    ${CMAKE_SOURCE_DIR}/seagull/headers
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glew/include
    ${CMAKE_SOURCE_DIR}/seagull/vendor/glm
    ${CMAKE_SOURCE_DIR}/seagull/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/seagull/vendor/stbimage)

file(GLOB PROJECT_SOURCES "seagull/src/*.cpp")
file(GLOB PROJECT_HEADERS "seagull/headers/*.h")
file(GLOB PROJECT_PYTHON_SOURCES "seagull/src/*.py")


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(${PROJECT_NAME} glfw glew glm assimp)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
