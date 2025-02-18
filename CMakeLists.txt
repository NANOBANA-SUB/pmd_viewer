cmake_minimum_required(VERSION 3.14)
project(PMD_VIEWER)

set(CMAKE_CXX_STANDARD 17)

# 実行ファイルを設定
set(SOURCE_FILES src/main.cpp src/Application.cpp src/PMDModel.cpp src/Shader.cpp src/Renderer.cpp)
add_executable(PMD_VIEWER ${SOURCE_FILES})

# --- SDL2の設定 ---
set(SDL2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/SDL2)
add_subdirectory(${SDL2_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SDL2_SOURCE_DIR}/include)
target_link_libraries(PMD_VIEWER PRIVATE SDL2-static SDL2main)

# --- GLEWの設定 (macOS は Homebrew を使用) ---
if(APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLEW REQUIRED glew)

    target_include_directories(PMD_VIEWER PRIVATE ${GLEW_INCLUDE_DIRS})
    
    # 明示的にフルパスのライブラリを指定
    target_link_libraries(PMD_VIEWER PRIVATE ${GLEW_LIBRARY_DIRS}/libGLEW.dylib)
endif()

# --- GLMの設定 ---
set(GLM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/glm)
target_include_directories(PMD_VIEWER PRIVATE ${GLM_SOURCE_DIR})

# --- SPDLOGの設定 ---
set(SPDLOG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/spdlog)
add_subdirectory(${SPDLOG_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SPDLOG_SOURCE_DIR}/include)
target_link_libraries(PMD_VIEWER PRIVATE spdlog)

# --- stbの設定 ---
set(STB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/stb)
target_include_directories(PMD_VIEWER PRIVATE ${STB_SOURCE_DIR})

# --- iconv の設定 ---
if(APPLE)
    target_link_libraries(PMD_VIEWER PRIVATE iconv)
else()
    find_library(ICONV_LIBRARY NAMES iconv PATHS ${CMAKE_PREFIX_PATH}/lib NO_DEFAULT_PATH)
    if(NOT ICONV_LIBRARY)
        message(FATAL_ERROR "libiconv not found!")
    endif()
    target_link_libraries(PMD_VIEWER PRIVATE ${ICONV_LIBRARY})
endif()

# --- OpenGLの設定 ---
if(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(PMD_VIEWER PRIVATE ${OpenGL_LIBRARY})
else()
    target_link_libraries(PMD_VIEWER PRIVATE opengl32)
endif()