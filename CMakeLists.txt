cmake_minimum_required(VERSION 3.15)
project(hw-gl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PKG_CONFIG_EXECUTABLE "/opt/homebrew/bin/pkg-config" CACHE FILEPATH "" FORCE)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

# macOS Framework
if(APPLE)
    find_library(COCOA_FRAMEWORK     Cocoa)
    find_library(IOKIT_FRAMEWORK     IOKit)
    find_library(COREVIDEO_FRAMEWORK CoreVideo)
    set(EXTRA_MAC_LIBS
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREVIDEO_FRAMEWORK}
    )
endif()

add_library(commonLib INTERFACE)
target_include_directories(commonLib INTERFACE
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    /opt/homebrew/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_directories(commonLib INTERFACE
    ${GLFW_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
)
target_link_libraries(commonLib INTERFACE
    ${OPENGL_LIBRARIES}
    ${GLFW_LDFLAGS}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${EXTRA_MAC_LIBS}
)

add_compile_definitions(GL_SILENCE_DEPRECATION)


# 改後

add_executable(window main.cpp)
target_link_libraries(window PRIVATE commonLib glm::glm)