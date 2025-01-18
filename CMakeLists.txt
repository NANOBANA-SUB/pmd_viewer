cmake_minimum_required(VERSION 3.14)
project(PMD_VIEWER)

set(CMAKE_CXX_STANDARD 14)

# 実行ファイルを設定
set(SOURCE_FILES src/main.cpp src/Application.cpp src/PMDModel.cpp src/Shader.cpp src/Renderer.cpp)
add_executable(PMD_VIEWER ${SOURCE_FILES})

# --- SDL2の設定 ---
set(SDL2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/SDL2)
add_subdirectory(${SDL2_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SDL2_SOURCE_DIR}/include)

# --- GLEWの設定 ---
set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/glew/include)
set(GLEW_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/third_party/glew/lib/Release/x64)
target_include_directories(PMD_VIEWER PRIVATE ${GLEW_INCLUDE_DIR})
target_link_directories(PMD_VIEWER PRIVATE ${GLEW_LIBRARY_DIR})
target_compile_definitions(PMD_VIEWER PRIVATE GLEW_STATIC)

# --- GLMの設定 ---
set(GLM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/glm)
target_include_directories(PMD_VIEWER PRIVATE ${GLM_SOURCE_DIR})

# ---SPDLOGの設定---
set(SPDLOG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/spdlog)
add_subdirectory(${SPDLOG_SOURCE_DIR} EXCLUDE_FROM_ALL)
target_include_directories(PMD_VIEWER PRIVATE ${SPDLOG_SOURCE_DIR}/include)

# --- ImGuiの設定 ---
set(IMGUI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

# ImGuiのソースファイルとバックエンドを指定
set(IMGUI_SOURCES
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# ImGuiのインクルードディレクトリを追加
target_include_directories(PMD_VIEWER PRIVATE 
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)

# ImGuiのソースをPMD_VIEWERに追加
target_sources(PMD_VIEWER PRIVATE ${IMGUI_SOURCES})

# --- ライブラリのリンク ---
target_link_libraries(PMD_VIEWER PRIVATE 
    SDL2-static SDL2main       # SDL2
    spdlog  # spdlog
    "${GLEW_LIBRARY_DIR}/glew32s.lib" # GLEW (静的リンク)
    opengl32                  # OpenGL (Windows標準)
)