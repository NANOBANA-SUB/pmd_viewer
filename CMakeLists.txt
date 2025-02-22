cmake_minimum_required(VERSION 3.14)
project(PMD_VIEWER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 初回ビルド時に Git サブモジュールを初期化・更新
execute_process(
    COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# 実行ファイルを設定
set(SOURCE_FILES src/main.cpp src/Application.cpp src/PMDModel.cpp src/Render/Shader.cpp src/Render/VertexArray.cpp src/Render/VertexBuffer.cpp src/Render/IndexBuffer.cpp src/Render/Texture.cpp src/Render/Render.cpp src/Render/FrameBuffer.cpp src/Viewer/GUI.cpp)
add_executable(PMD_VIEWER ${SOURCE_FILES})

# --- SDL2 の設定 ---
set(SDL2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/SDL2)
add_subdirectory(${SDL2_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SDL2_SOURCE_DIR}/include)
target_link_libraries(PMD_VIEWER PRIVATE SDL2-static SDL2main)

# --- GLEW の設定 ---
if(APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLEW REQUIRED glew)
    target_include_directories(PMD_VIEWER PRIVATE ${GLEW_INCLUDE_DIRS})
    target_link_libraries(PMD_VIEWER PRIVATE ${GLEW_LIBRARY_DIRS}/libGLEW.dylib)
else()
    set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/glew/include)
    set(GLEW_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/third_party/glew/lib/Release/x64)
    target_include_directories(PMD_VIEWER PRIVATE ${GLEW_INCLUDE_DIR})
    target_link_directories(PMD_VIEWER PRIVATE ${GLEW_LIBRARY_DIR})
    target_compile_definitions(PMD_VIEWER PRIVATE GLEW_STATIC)
    target_link_libraries(PMD_VIEWER PRIVATE "${GLEW_LIBRARY_DIR}/glew32s.lib")
endif()

# --- GLM の設定 ---
set(GLM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/glm)
target_include_directories(PMD_VIEWER PRIVATE ${GLM_SOURCE_DIR})

# --- SPDLOG の設定 ---
set(SPDLOG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/spdlog)
add_subdirectory(${SPDLOG_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SPDLOG_SOURCE_DIR}/include)
target_link_libraries(PMD_VIEWER PRIVATE spdlog)

# --- stb の設定 ---
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

# --- ImGuiの設定 ---
set(IMGUI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

set(IMGUI_SOURCES
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(PMD_VIEWER PRIVATE
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)

target_sources(PMD_VIEWER PRIVATE ${IMGUI_SOURCES})

# --- OpenGL の設定 ---
if(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(PMD_VIEWER PRIVATE ${OpenGL_LIBRARY})
else()
    target_link_libraries(PMD_VIEWER PRIVATE opengl32)
endif()
